//
// Created by rookie on 2025/1/6.
//
#include "structbind1.hpp"
#include <iostream>
int main()
{
    Customer c{"Tim", "Starr", 42};
    auto [f, l, v] = c;
    std::cout << "f/l/v:" << f << ' ' << l << ' ' << v << '\n';
// 修 改 结 构 化 绑 定 的 变 量
    std::string s{std::move(f)};
    l = "Waters";
    v += 10;
    std::cout << "f/l/v:" << f << ' ' << l << ' ' << v << '\n';
    std::cout << "c:" << c.getFirst() << ' '
              << c.getLast() << ' ' << c.getValue() << '\n';
    std::cout << "s:" << s << '\n';
}