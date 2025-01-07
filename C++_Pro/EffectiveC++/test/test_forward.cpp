//
// Created by rookie on 23-12-17.
// 万能引用与完美转发的学习
//
#include <iostream>

using namespace std;

void foo(int &&i) {} // i为右值引用

template<class T>
void bar(T &&t) {
    T x = t;
    std::cout << typeid(T).name() <<"\n";
}

int get_val() { return 5; }

int main()
{
    int i = 42;
    const int j = 11;
    bar(i);
    bar(j);
    bar(get_val());
    auto &&x = i;
    auto &&y = j;
    auto &&z = get_val();

}