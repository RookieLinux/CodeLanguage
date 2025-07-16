//
// Created by rookie on 2025/1/6.
//

#ifndef TEST_STRUCTURED_BINDINGS_STRUCTBIND1_HPP
#define TEST_STRUCTURED_BINDINGS_STRUCTBIND1_HPP

#include "customer1.hpp"
#include <utility>
// for tuple-like API
// 为 类 Customer 提 供 tulpe-like API ：
template<>
struct std::tuple_size<Customer> {
    static constexpr int value = 3; // 有 三 个 属 性
};
template<>
struct std::tuple_element<2, Customer> {
    using type = long;
// 最 后 一 个 属 性 的 类 型 是 long
};
template<std::size_t Idx>
struct std::tuple_element<Idx, Customer> {
    using type = std::string;
// 其 他 的 属 性 都 是 string
};
// 定 义 特 化 的 getter ：
template<std::size_t> auto get(const Customer& c);
template<> auto get<0>(const Customer& c) { return c.getFirst(); }
template<> auto get<1>(const Customer& c) { return c.getLast(); }
template<> auto get<2>(const Customer& c) { return c.getValue(); }
/*
template<std::size_t I> auto get(const Customer& c) {
    static_assert(I < 3);
    if constexpr (I == 0) {
        return c.getFirst();
    }else if constexpr (I == 1) {
        return c.getLast();
    }else {
        // I == 2
        return c.getValue();
    }
}
*/
#endif //TEST_STRUCTURED_BINDINGS_STRUCTBIND1_HPP
