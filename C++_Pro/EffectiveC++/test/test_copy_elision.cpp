//
// Created by rookie on 2025/1/7.
//
/*
    https://www.cnblogs.com/chen-pi/p/17931460.html  条款25
    在 C++ 计算机编程中，复制省略（Copy elision）是指一种编译器优化技术，它消除了不必要的对象复制。
    常见的俩种场景下复制省略：
    1、纯右值参数复制构造
    2、函数返回值优化（Return value optimization RVO)
*/
#include <iostream>

int num = 0;
class X{
public:
    explicit X(int) {
        std::cout << "Call X(int)" << std::endl;
    }
    X(const X&) {
        num++;
        std::cout << "Call X(const x&)" << std::endl;
    }
};


int num1 = 0;
class X1{
public:
    X1() = default;
    X1(const X1&) {
        num1++;
        std::cout << "Call X1(const x1&)" << std::endl;
    }

#if 1
    X1(const X1&&)  noexcept {
        num1++;
        std::cout << "Call X1(const x1&&)" << std::endl;
    }
#else
    X1(const X1&&) = delete;
#endif
};

X1 return_unknow_value() {
    return X1();
}


struct MyType {
    char buffer[100000];
};


MyType return_name_value(bool test) {
    if (test) {
        MyType x;
        x.buffer[0] = '\0';
        return x;
    } else {
        MyType y;
        return y;
    }
}

int main() {
    /*
        简单来讲：
        • 所有名称都是 lvalue(左值)。
        • 所有用作表达式的字符串字面量是 lvalue(左值)。
        • 所有其他的字面量（4.2, true, nullptr）是 prvalue(纯右值)。
        • 所有临时对象（尤其是以值返回的对象）是 prvalue(纯右值)。
    */
    /* 纯右值参数复制构造 */
    X x1(42);     //直接调用构造函数初始化
    X x2 = X(42); //C++17中，同类型纯右值复制构造,复制省略，直接在x2位置上进行构造
    X x3 = x1;    //左值复制构造
    std::cout << num << std::endl;   //1,只在左值复制调用了一次

    /*
       RVO是一种编译器优化技术，在接收对象的位置构造返回值，避免从函数返回时创建临时对象。
       到了C++17标准保证了函数返回临时对象不会被复制，而不再是依赖于编译器优化

       RVO 相对于不同版本编译器和C++标准区别
       1、C++17标准保证返回临时对象不会发生拷贝，使用C++17标准无需担心可能出现拷贝的情况。
       2、C++17之前标准，返回类型对象是可移动的，存在移动构造函数（定义或默认生成），否则编译会报错，而C++17标准无论移动构造和移动赋值是否被删除都会进行RVO优化。
       3、C++17之前标准 gcc 可以添加-fno-elide-constructors 编译参数来禁止编译进行RVO优化。
    */
    auto x = return_unknow_value();
    std::cout << num1 << std::endl;  //0,返回值优化

     /*
        NRVO与RVO类似，但适用于返回函数内部已命名的局部变量。
        编译器优化这个过程，允许在调用者的栈帧上直接构造局部变量，避免了将局部变量拷贝到返回值的过程，但是NRVO并不能保证每次都会进行优化，在有一些情况不会发生，不同编译器情况也不太一样，依赖于编译器实现。
        gcc编译器：当test为true时，gcc会在栈上重新分配内存，然后在将内存拷贝到之前的返回值地址上。当test为false时，发生NRVO优化
        clang编译器：能够处理分支并实现NRVO优化
        1. 当返回值是具名局部变量时,是否能进行NRVO优化主要依赖于具体编译器的实现
        2. 返回函数参数不会发生NRVO优化
        3. 返回值是全局变量也不会发生优化
        4. 返回值使用move进行转换也不会发生优化
     */
    auto my_type = return_name_value(true);

    return 0;
}