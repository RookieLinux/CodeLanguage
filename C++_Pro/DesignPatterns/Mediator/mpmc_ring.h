// mpmc_ring.h
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <new>
#include <type_traits>

// Simple Vyukov MPMC bounded queue adapted for T (T default constructible or trivially assignable).
template<typename T>
class MPMCRing {
    static_assert(std::is_default_constructible<T>::value || std::is_trivially_copyable<T>::value,
                  "T must be default constructible or trivially copyable for this implementation.");

public:
    explicit MPMCRing(size_t capacity)
        : _capacity(round_up_pow2(capacity)),
          _mask(_capacity - 1),
          _buffer(static_cast<Slot*>(operator new[](_capacity * sizeof(Slot))))
    {
        assert(_capacity >= 2);
        for (size_t i = 0; i < _capacity; ++i) {
            new(&_buffer[i]) Slot();
            _buffer[i].sequence.store(i, std::memory_order_relaxed);
        }
        _enqueuePos.store(0, std::memory_order_relaxed);
        _dequeuePos.store(0, std::memory_order_relaxed);
        _dropCount.store(0, std::memory_order_relaxed);
    }

    ~MPMCRing() {
        for (size_t i = 0; i < _capacity; ++i) {
            _buffer[i].~Slot();
        }
        operator delete[](_buffer);
    }

    MPMCRing(const MPMCRing&) = delete;
    MPMCRing& operator=(const MPMCRing&) = delete;

    bool try_push(const T& item) {
        return emplace_impl(item);
    }

    bool try_push(T&& item) {
        return emplace_impl(std::move(item));
    }

    template<typename... Args>
    bool emplace(Args&&... args) {
        return emplace_impl(std::forward<Args>(args)...);
    }

    bool try_pop(T& out) {
        Slot* slot;
        size_t pos = _dequeuePos.load(std::memory_order_relaxed);
        for (;;) {
            slot = &_buffer[pos & _mask];
            size_t seq = slot->sequence.load(std::memory_order_acquire);
            intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos + 1);
            if (diff == 0) {
                if (_dequeuePos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    out = std::move(slot->storage);
                    slot->sequence.store(pos + _capacity, std::memory_order_release);
                    return true;
                }
            } else if (diff < 0) {
                return false; // empty
            } else {
                pos = _dequeuePos.load(std::memory_order_relaxed);
            }
        }
    }

    size_t approximate_size() const {
        size_t enq = _enqueuePos.load(std::memory_order_acquire);
        size_t deq = _dequeuePos.load(std::memory_order_acquire);
        return (enq >= deq) ? (enq - deq) : 0;
    }

    uint64_t drop_count() const {
        return _dropCount.load(std::memory_order_acquire);
    }

    void reset_drop_count() {
        _dropCount.store(0, std::memory_order_release);
    }

    size_t capacity() const { return _capacity; }

private:
    struct Slot {
        std::atomic<size_t> sequence;
        T storage;
        Slot() : sequence(0), storage() {}
        ~Slot() = default;
    };

    static size_t round_up_pow2(size_t v) {
        if (v <= 2) return 2;
        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
#if SIZE_MAX > UINT32_MAX
        v |= v >> 32;
#endif
        ++v;
        return v;
    }

    template<typename... Args>
    bool emplace_impl(Args&&... args) {
        Slot* slot;
        size_t pos = _enqueuePos.load(std::memory_order_relaxed);
        for (;;) {
            slot = &_buffer[pos & _mask];
            size_t seq = slot->sequence.load(std::memory_order_acquire);
            intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);
            if (diff == 0) {
                if (_enqueuePos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                    slot->storage = T(std::forward<Args>(args)...);
                    slot->sequence.store(pos + 1, std::memory_order_release);
                    return true;
                }
            } else if (diff < 0) {
                _dropCount.fetch_add(1, std::memory_order_relaxed);
                return false; // full
            } else {
                pos = _enqueuePos.load(std::memory_order_relaxed);
            }
        }
    }

private:
    const size_t _capacity;
    const size_t _mask;
    Slot* _buffer;

    alignas(64) std::atomic<size_t> _enqueuePos{0};
    alignas(64) std::atomic<size_t> _dequeuePos{0};
    std::atomic<uint64_t> _dropCount;
};

