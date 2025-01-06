//
// Created by rookie on 2025/1/6.
//

#ifndef TEST_STRUCTURED_BINDINGS_STRUCTBIND2_HPP
#define TEST_STRUCTURED_BINDINGS_STRUCTBIND2_HPP

#include "customer2.hpp"
#include <utility>
// for tuple-like API
// 为 类 Customer 提 供 tuple-like API
template<>
struct std::tuple_size<Customer> {
    static constexpr int value = 3; // 有 3 个 属 性
};
template<>
struct std::tuple_element<2, Customer> {
    using type = long;
// 最 后 一 个 属 性 是 long 类 型
};

//template<>
//struct std::tuple_element<1, Customer> {
//    using type = const std::string;
//};

template<std::size_t Idx>
struct std::tuple_element<Idx, Customer> {
        using type = std::string;
};
// 定 义 特 化 的 getter ：
template<std::size_t I> decltype(auto) get(Customer& c) {
    static_assert(I < 3);
    if constexpr (I == 0) {
        return c.firstname();
    }
    else if constexpr (I == 1) {
        return c.lastname();
    }
    else {
// I == 2
        return c.value();
    }
}
template<std::size_t I> decltype(auto) get(const Customer& c) {
    static_assert(I < 3);
    if constexpr (I == 0) {
        return c.firstname();
    }
    else if constexpr (I == 1) {
        return c.lastname();
    }
    else {
// I == 2
        return c.value();
    }
}

template<std::size_t I> decltype(auto) get(Customer&& c) {
    static_assert(I < 3);
    if constexpr (I == 0) {
        return std::move(c.firstname());
    }
    else if constexpr (I == 1) {
        return std::move(c.lastname());
    }
    else {
// I == 2
        return c.value();
    }
}
#endif //TEST_STRUCTURED_BINDINGS_STRUCTBIND2_HPP
