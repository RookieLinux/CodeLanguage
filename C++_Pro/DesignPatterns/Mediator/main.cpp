// main.cpp
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <cstring>
#include <signal.h>
#include "mediator_router.h"

// configuration
constexpr size_t PRODUCERS = 12;
constexpr size_t CONSUMERS = 16;
using Item = int;

std::atomic<bool> keepRunning{true};

void sigint_handler(int) {
    keepRunning.store(false);
}

int main(int argc, char** argv) {
    signal(SIGINT, sigint_handler);

    size_t perConsumerCapacity = 2000000; // default
    size_t testSeconds = 10;

    if (argc >= 2) {
        perConsumerCapacity = std::stoul(argv[1]);
    }
    if (argc >= 3) {
        testSeconds = std::stoul(argv[2]);
    }

    std::cout << "Starting mediator test. Producers=" << PRODUCERS
              << " Consumers=" << CONSUMERS
              << " perConsumerCapacity=" << perConsumerCapacity
              << " testSeconds=" << testSeconds << std::endl;

    MediatorRouter<Item, PRODUCERS, CONSUMERS> router(perConsumerCapacity, 128);

    // for demonstration: connect each producer to all consumers (full fan-out)
    for (size_t p = 0; p < PRODUCERS; ++p) {
        std::vector<ConsumerId> vec;
        for (size_t c = 0; c < CONSUMERS; ++c) vec.push_back(c);
        router.updateRoutes(p, vec);
    }

    // stats
    std::atomic<uint64_t> producedCount{0};
    std::atomic<uint64_t> consumedCount{0};
    std::atomic<uint64_t> totalBlockedNs{0};

    // launch consumer threads
    std::vector<std::thread> consumers;
    consumers.reserve(CONSUMERS);
    for (size_t c = 0; c < CONSUMERS; ++c) {
        consumers.emplace_back([c, &router, &consumedCount]() {
            Item x;
            while (keepRunning.load()) {
                if (router.consume(c, x)) {
                    consumedCount.fetch_add(1, std::memory_order_relaxed);
                    // simulate processing cost minimal (int) - in real app do work here
                }
            }
            // drain remaining items after stop signal
            while (router.consumer_ring(c)->try_pop(x)) {
                consumedCount.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // launch producer threads
    std::vector<std::thread> producers;
    producers.reserve(PRODUCERS);
    for (size_t p = 0; p < PRODUCERS; ++p) {
        producers.emplace_back([p, &router, &producedCount, &totalBlockedNs]() {
            uint64_t localBlocked = 0;
            Item v = 0;
            // producers continually send until test end
            while (keepRunning.load()) {
                uint64_t blockedNs = 0;
                router.send(p, v, blockedNs);
                if (blockedNs) localBlocked += blockedNs;
                producedCount.fetch_add(1, std::memory_order_relaxed);
                ++v;
                // no sleep: push as fast as possible to saturate
            }
            // accumulate
            totalBlockedNs.fetch_add(localBlocked, std::memory_order_relaxed);
        });
    }

    // run for requested seconds
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t s = 0; s < testSeconds && keepRunning.load(); ++s) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        uint64_t p = producedCount.load();
        uint64_t c = consumedCount.load();
        std::cout << "[stats] elapsed=" << (s+1) << "s produced=" << p << " consumed=" << c << std::endl;
    }

    keepRunning.store(false);

    // join producers
    for (auto &t : producers) if (t.joinable()) t.join();
    // notify consumers to wake and drain
    for (size_t i = 0; i < CONSUMERS; ++i) {
        // notify consumers in case they are waiting
        // we don't have direct access to CV here, but consume pop will eventually drain
    }
    for (auto &t : consumers) if (t.joinable()) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    double secs = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    uint64_t pcount = producedCount.load();
    uint64_t ccount = consumedCount.load();
    uint64_t blockedNs = totalBlockedNs.load();
    double blockedSec = double(blockedNs) / 1e9;

    std::cout << "TEST DONE\n";
    std::cout << "Elapsed seconds: " << secs << std::endl;
    std::cout << "Produced total: " << pcount << std::endl;
    std::cout << "Consumed total: " << ccount << std::endl;
    std::cout << "Total producer blocked time (s): " << blockedSec << std::endl;

    return 0;
}

