//
// Created by rookie on 24-12-6.
//
/*
 * C++ 什么时候用.template 和 ::template
 * 简单来说，就是你有一个未知类型T（这个T本身就是模板）
 * 假设这个T是一个类，这个类里包含了一些模板函数或者模板结构体
 * 你需要使用T. 或者 T:: 去调用他们，并且要显示指定模板参数
 * 这个时候就需要用到.template 和 ::template
 * */
#include  <iostream>
struct A {
    void Foo() { std::cout << __PRETTY_FUNCTION__ << std::endl; }

    template <typename T>
    void Foo() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    template <typename T>
    static void Goo() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    template <typename T>
    struct B {
        using type = T;
        void Foo() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
        template <typename U>
        void Foo() {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }
    };

    struct C {
        void Foo() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    };
};

template <typename T>
struct Test {
    void Fun(T t) {
        t.Foo();
        t.template Foo<int>();

        t.template Goo<int>();
        T::template Goo<int>();

        typename T::template B<int>::type a;
        typename T::template B<int>().Foo();
        typename T::template B<int>().template Foo<int>();

        typename T::C().Foo();
    }
};

int main() {
    A a;
    Test<A>().Fun(a);
}
