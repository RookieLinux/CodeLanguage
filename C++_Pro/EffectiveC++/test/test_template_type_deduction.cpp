//
// Created by rookie on 2025/1/8.
//
/*
 * 对于这样一个函数模板：
 * template<typename T>
 * void f(ParamType param);
 * 它的调用是这样：
 * f(expr);
 *
 * T的推导不仅取决于expr的类型，也取决于ParamType的类型。
 * 根据ParamType的类型，将推导分为三种情况：
 * 1. ParamType是⼀个指针或引⽤，但不是万能引⽤
 *    a. 如果expr的类型是⼀个引⽤，忽略引⽤部分
 *    b. 然后剩下的部分决定T，然后T与形参匹配得出最终ParamType
 * 2. ParamType是⼀个万能引用
 *    a. 如果expr是左值，T和ParamType都会被推导为左值引⽤
 *    b. 如果expr是右值，就使⽤情况一的推导规则
 * 3. ParamType既不是指针也不是引用
 *    a. 如果expr的类型是⼀个引⽤，忽略引⽤部分
 *    b. 如果忽略引⽤之后expr是⼀个const，那就再忽略const。如果它是volatile，也会被忽略
*/

#include <iostream>
using namespace std;

template<typename T>
void f1(T & param)
{

}

template<typename T>
void f2(T&& param)
{

}

template<typename T>
void f3(T param)
{

}

int main()
{
    /* 情况一 */
    int x1=27;
    const int cx1=x1;
    const int & rx1=cx1;
    f1(x1);       //T是int; param是int&
    f1(cx1);  //T是const int; param是const int&
    f1(rx1);  //T是const int; param是const int&

    /* 情况二 */
    int x2=27;
    const int cx2=x2;
    const int & rx2=cx2;
    f2(x2);       //T是int&; param是int&
    f2(cx2);  //T是const int&; param是const int&
    f2(rx2);  //T是const int&; param是const int&
    f2(27);   //T是int; param是int&&

    /* 情况三 */
    int x3=27;
    const int cx3=x2;
    const int & rx3=cx2;
    f3(x2);   //T是int; param是int
    f3(cx2);  //T是int; param是int
    f3(rx2);  //T是int; param是int

    return 0;
}

