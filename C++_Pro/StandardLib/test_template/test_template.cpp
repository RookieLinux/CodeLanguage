#include <iostream>

using namespace std;
/*
                                                            typename关键字说明
      使用场合  	                    示例代码	                                                            描述
    模板参数列表	template <typename T> void function(T param);	                        typename 在此处定义了 T 作为一个类型模板参数。
    函数参数	    template <typename T> void function(typename T::SubType param);	        typename 在此处明确指定 T::SubType 是从模板参数 T 中派生的类型。
    函数返回值	template <typename T> typename T::SubType function();	                typename 在此处明确指定 T::SubType 是从模板参数 T 中派生的类型，并且是函数的返回类型。
    类成员	    template <typename T> class MyClass { typename T::SubType member; };	typename 在此处明确指定 T::SubType 是从模板参数 T 中派生的类型，并且是类 MyClass 的成员的类型。
*/

template <typename T>
struct MyContainer {
    typedef T* iterator;  // 使用 typedef 定义了一个迭代器类型别名

    void func() {
        typename T::NestedType t;  // 使用 typename 引用 T 的嵌套类型（class不可嵌套）
        // ...
    }
};

/*
 *  模板特例化
 */
/* 全特化即将模板类型里的所有类型参数全部具体指明之后处理 */
template <typename T>
class MyClass {
public:
    typedef T NestedType;
};

template <>
class MyClass<int> {
public:
    typedef int NestedType;
};

/* 类模板偏特化（局部特化）：顾名思义，只特殊化几个参数或者一定的参数范围 */
template<typename T,typename C,typename D>
struct A
{
    void func()
    {
        cout << "泛化版本" << endl;
    }
};
template<typename C>
struct A<int,C,int> //个数偏特化
{
    void func()
    {
        cout << "int,C,int偏特化版本" << endl;
    }
};
template<typename C>
struct A<double, C, double> //个数偏特化
{
    void func()
    {
        cout << "double,C,double偏特化版本" << endl;
    }
};
// 比如const int属于int的一个小范围，
// int *和const int*属于int的一个小范围，
// int&属于int的一个小范围，int&&属于int的一个小范围
template<typename T>
struct B
{
    void func()
    {
        cout << "泛化版本" << endl;
    }
};
template<typename T>
struct B<const T> //范围偏特化
{
    void func()
    {
        cout << "const T版本" << endl;
    }
};
template<typename T>
struct B<T*> //范围偏特化
{
    void func()
    {
        cout << "T*版本" << endl;
    }
};
template<typename T>
struct B<T&> //范围偏特化
{
    void func()
    {
        cout << "T&版本" << endl;
    }
};

/*
 *                      typename多层嵌套
 * 在这个例子中，func函数接收一个类型为Outer<T>::Inner<U>的参数。
 * 由于Outer<T>::Inner<U>是一个依赖于模板参数的类型，因此我们需要在前面加上typename关键字。
 * 同时，由于Inner是Outer<T>的一个模板成员，所以我们需要在Inner前面加上template关键字。
 */
template<typename T>
class Outer {
public:
    template<typename U>
    class Inner {};
};

template<typename T, typename U>
void func(typename Outer<T>::template Inner<U> param) {
    // 实现细节...
}

//注意：函数模板不能偏特化！！！！！！！！！！！！！
template<typename T,typename C>
void func(T &a, C &b)
{
    cout << "泛化版本 ";
    cout << a <<" ";
    cout << b << endl;
}

template<>
void func(int &a, int &b)
{
    cout << "int,int特化版本 ";
    cout << a <<" ";
    cout << b << endl;
}

void func(int &a, int &b)
{
    cout << "重载版本 ";
    cout << a <<" ";
    cout << b << endl;
}

int main() {
    std::cout << "---类模板测试---" << std::endl;

    A<double, int, int> a;
    a.func();//调用泛化版本
    A<int, char, int> b;
    b.func();//调用int,C,int偏特化版本
    A<double, int, double> c;
    c.func();//调用double,C,double偏特化版本

    std::cout << "---函数模板测试---" << std::endl;
    double da = 1.0;
    double db = 2.0;
    func(da, db);
    int ic = 3;
    int id = 4;
    func(ic, id);//不特地指明参数类型时，编译器会优先选择重载普通函数版本
    int e = 3;
    int f = 4;
    func<int, int>(e, f);

    return 0;
}
