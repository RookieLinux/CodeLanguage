#include <iostream>
#include <type_traits> // 存在is_rvalue_reference、is_lvalue_reference、is_reference模板

struct A {
    double x;
};

template<class T>
T sum1(T a1, T a2)
{
    return a1 + a2;
}

template<class R, class T1, class T2>
R sum2(T1 a1, T2 a2)
{
    return a1 + a2;
}

template<class T1, class T2>
auto sum3(T1 a1, T2 a2)->decltype(a1 + a2)
{
    return a1 + a2;
}

//c++14
//template<class T>
//auto return_ref(T& t)
//{
//    return t;
//}

int main() {

    std::cout << "decltype 推导规则" << std::endl;
    int x = 0;
    double y = 5.5;
    std::cout << typeid(x).name() << std::endl;
    std::cout << typeid(y).name() << std::endl;
//    1．typeid的返回值是一个左值，且其生命周期一直被扩展到程序生命周期结束。
    auto &t2 = typeid(int); // 编译成功，t2推导为const std::type_info&
    auto t3 = &typeid(int); // 编译成功，t3推导为const std::type_info*

//    2．typeid返回的std::type_info删除了复制构造函数，若想保存std::type_info，只能获取其引用或者指针，例如：
//    auto t1 = typeid(int); // 编译失败，没有复制构造函数无法编译

//    3．typeid的返回值总是忽略类型的 cv 限定符，也就是typeid(const T)== typeid(T))。

    std::cout << "decltype 推导规则" << std::endl;

    auto x1 = sum1(5, 10);
    //auto x2 = sum1(5, 10.5); // 编译失败，无法确定T的类型
    auto x3 = sum2<double>(5, 10.5);
    auto x4 = sum3(5, 10.5);

//    1．如果e是一个未加括号的标识符表达式（结构化绑定除外）或者未加括号的类成员访问，则decltype(e)推断出的类型是e的类型T。
//    如果并不存在这样的类型，或者e是一组重载函数，则无法进行推导。
//    2．如果e是一个函数调用或者仿函数调用，那么decltype(e)推断出的类型是其返回值的类型。
//    3．如果e是一个类型为T的左值，则decltype(e)是T&。
//    4．如果e是一个类型为T的将亡值，则decltype(e)是T&&。
//    5．除去以上情况，则decltype(e)是T。
    int i;
    int *j;
    int n[10];
    decltype(i=0) a = i; //规则3
    decltype(0,i) b = i; //规则3
    decltype(i,0) c; //规则5
    decltype(n[5]) d = i; //规则3
    decltype(*j) e= i; //规则5
//    decltype(static_cast<int&&>(i)) iii; //规则4
    decltype(i++) g; //规则5
    decltype(++i) h= i; //规则3
    decltype("hello world") jjj = "hello world";

//    6. 通常情况下，decltype(e)所推导的类型会同步e的cv限定符，但是还有其他情况，当e是未加括号的成员变量时，父对象表达式的cv限定符会被忽略，不能同步到推导结果：
    const int ia = 1;
//    decltype(ia); // decltype(i)推导类型为const int
    const A* pa = new A();
//    decltype(pa->x);// decltype(pa->x)推导类型为double, const属性被忽略
//    decltype((pa->x)); //规则6
    if(pa)
    {
        delete pa;
        pa = nullptr;
    }
    return 0;
}
