// mediator_router.h
#pragma once

#include <atomic>
#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstdint>
#include <cassert>
#include "mpmc_ring.h"

using ProducerId = size_t;
using ConsumerId = size_t;

// MediatorRouter:
// - PRODUCERS producers and CONSUMERS consumers
// - each consumer has its own MPMCRing<T>
// - routing snapshot (RCU-style) maps producer -> vector<consumer>
// - producers do fan-out: for each target consumer, try fast push, otherwise block on per-consumer CV
template<typename T, size_t PRODUCERS, size_t CONSUMERS>
class MediatorRouter {
public:
    struct RouteSnapshot {
        std::array<std::vector<ConsumerId>, PRODUCERS> mapping;
    };

    MediatorRouter(size_t perConsumerRingSize,
                   size_t fastSpin = 128)
        : _fastSpin(fastSpin)
    {
        for (size_t i = 0; i < CONSUMERS; ++i) {
            consumerRings[i] = new MPMCRing<T>(perConsumerRingSize);
        }
        // initialize snapshot with an empty RouteSnapshot
        auto initial = std::make_shared<RouteSnapshot>();
        std::atomic_store(&snapshot, initial); // atomic_store for shared_ptr
    }

    ~MediatorRouter() {
        for (size_t i = 0; i < CONSUMERS; ++i) {
            delete consumerRings[i];
        }
    }

    // RCU snapshot update (copy-on-write for a single producer)
    void updateRoutes(ProducerId p, const std::vector<ConsumerId>& newConsumers) {
        assert(p < PRODUCERS);
        // load current snapshot atomically
        auto old = std::atomic_load(&snapshot);
        // copy, modify the copy, then publish with atomic_store
        auto nw = std::make_shared<RouteSnapshot>(*old);
        nw->mapping[p] = newConsumers;
        std::atomic_store(&snapshot, nw);
    }

    void connect(ProducerId p, ConsumerId c) {
        assert(p < PRODUCERS && c < CONSUMERS);
        auto old = std::atomic_load(&snapshot);
        auto nw = std::make_shared<RouteSnapshot>(*old);
        auto &vec = nw->mapping[p];
        if (std::find(vec.begin(), vec.end(), c) == vec.end()) vec.push_back(c);
        std::atomic_store(&snapshot, nw);
    }

    void disconnect(ProducerId p, ConsumerId c) {
        assert(p < PRODUCERS && c < CONSUMERS);
        auto old = std::atomic_load(&snapshot);
        auto nw = std::make_shared<RouteSnapshot>(*old);
        auto &vec = nw->mapping[p];
        vec.erase(std::remove(vec.begin(), vec.end(), c), vec.end());
        std::atomic_store(&snapshot, nw);
    }

    // Producer send: block until every target consumer accepted the item (no drop)
    // Returns true if succeeded.
    bool send(ProducerId p, const T &item, uint64_t &blockedNanoseconds) {
        assert(p < PRODUCERS);
        auto snap = std::atomic_load(&snapshot);
        const auto &targets = snap->mapping[p];
        blockedNanoseconds = 0;

        for (ConsumerId c : targets) {
            // fast spin attempts
            bool pushed = false;
            for (size_t i = 0; i < _fastSpin; ++i) {
                if (consumerRings[c]->try_push(item)) {
                    pushed = true;
                    break;
                }
                std::this_thread::yield();
            }
            if (pushed) {
                // notify consumer that data is available
                notify_consumer_not_empty(c);
                continue;
            }

            // slow path: block on CV until push succeeds
            auto start = std::chrono::high_resolution_clock::now();
            std::unique_lock<std::mutex> lk(cvMutex[c]);
            while (!consumerRings[c]->try_push(item)) {
                // Wait with timeout to be resilient; wakeups will re-check
                cvNotFull[c].wait_for(lk, std::chrono::milliseconds(1));
            }
            auto end = std::chrono::high_resolution_clock::now();
            blockedNanoseconds += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

            // notify consumer new data
            notify_consumer_not_empty(c);
        }
        return true;
    }

    // Consumer blocking pop. After successful pop, notifies producers waiting for space.
    bool consume(ConsumerId c, T &out) {
        assert(c < CONSUMERS);
        // fast try loop first
        if (consumerRings[c]->try_pop(out)) {
            notify_consumer_not_full(c);
            return true;
        }
        // otherwise block on not-empty CV
        std::unique_lock<std::mutex> lk(cvMutex[c]);
        while (!consumerRings[c]->try_pop(out)) {
            cvNotEmpty[c].wait_for(lk, std::chrono::milliseconds(1));
        }
        // after successful pop notify potential producers waiting for free slot
        notify_consumer_not_full(c);
        return true;
    }

    MPMCRing<T>* consumer_ring(ConsumerId c) {
        assert(c < CONSUMERS);
        return consumerRings[c];
    }

private:
    void notify_consumer_not_empty(ConsumerId c) {
        // notify consumer thread that there is data
        cvNotEmpty[c].notify_one();
    }

    void notify_consumer_not_full(ConsumerId c) {
        // notify producers waiting because consumer ring had been full
        cvNotFull[c].notify_one();
    }

private:
    // snapshot is a plain shared_ptr but accesses use atomic_load/atomic_store
    std::shared_ptr<RouteSnapshot> snapshot;

    MPMCRing<T>* consumerRings[CONSUMERS];

    std::mutex cvMutex[CONSUMERS];
    std::condition_variable_any cvNotEmpty[CONSUMERS];
    std::condition_variable_any cvNotFull[CONSUMERS];

    size_t _fastSpin;
};

