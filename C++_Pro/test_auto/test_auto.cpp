#include <iostream>
#include <map>

/********************************* 用法 *********************************/
int sum(int a1, int a2)
{
    return a1+a2;
}

class Base {
public:
    virtual void f()
    {
        std::cout << "Base::f()" << std::endl;
    };
};
class Derived : public Base {
public:
    void f() override
    {
        std::cout << "Derived::f()" << std::endl;
    };
};


/********************************* 规则 *********************************/
//7．C++11标准支持auto声明静态成员变量，但是必须使用const限定符，在C++17标准中，对于静态成员变量，auto可以在没有const的情况下使用
//    struct sometype {
//        auto i = 5;
//    };// 错误，无法编译通过
struct sometype {
    static const auto i = 5;
};
//struct sometype {
//    static auto i = 5;
//};


// 8．按照C++20之前的标准，无法在函数形参列表中使用auto声明形参（注意，在C++14中，auto可以为lambda表达式声明形参）
//void echo(auto str)
//{
//
//}

// 9. C++17标准对auto关键字又一次进行了扩展，使它可以作为模板形参的占位符（注意：double等类型不可以）
//template<auto N>
//void f()
//{
//    std::cout << N << std::endl;
//}

int main() {

    std::cout << "---auto 推导规则---" << std::endl;

    // 1．当用一个auto关键字声明多个变量的时候，编译器遵从由左往右的推导规则，以最左边的表达式推断auto的具体类型
    int n = 5;
    auto *pn = &n, m = 10;
    //auto *pn = &n, m = 10.0;// 编译失败，声明类型不统一

    // 2．当使用条件表达式初始化auto声明的变量时，编译器总是使用表达能力更强的类型
    auto i = true ? 5 : 8.0;// i的数据类型为double

   // 3．如果auto声明的变量是按值初始化，且没有使用引用，也没有使用指针，则推导出的类型会忽略cv限定符
    const int o = 5;
    auto p = o;                // auto推导类型为int，而非const int
    auto &q = o;         // auto推导类型为const int，q推导类型为const int&
    auto *r = &o;        // auto推导类型为const int，r推导类型为const int*
    const auto s = p;     // auto推导类型为int，s的类型为const int
   // 4.使用auto声明变量初始化时，目标对象如果是引用，则引用属性会被忽略
    int t = 5;
    int &u = t;
    auto v = u; // auto推导类型为int，而非int&
    // 5．使用auto和万能引用声明变量时，对于左值会将auto推导为引用类型
    int w = 5;
    auto&& x = w; // auto推导类型为int& （这里涉及引用折叠的概念）
    auto&& y = 5;       // auto推导类型为int
    // 6．使用auto声明变量，如果目标对象是一个数组或者函数，则auto会被推导为对应的指针类型
    int z[5];
    auto a = z;          // auto推导类型为int*
    auto b = sum;  // auto推导类型为int(*)(int,int)

    std::cout << "---auto 结合多态---" << std::endl;
    Base* c = new Derived();
    c->f();
    auto d = *c;
    d.f();
    auto &bbb = *c;
    bbb.f();

    std::cout << "---auto 用法---" << std::endl;
    std::map<std::string, int> str2int;
    //填充str2int的代码
    for (auto it = str2int.cbegin(); it != str2int.cend(); ++it)
    {

    }
    for (auto &it : str2int)
    {

    }

    // C++14标准支持对返回类型声明为auto的推导，例如：
//    auto sum(int a1, int a2)
//    {
//        return a1 + a2;
//    }
    //在C++14标准中我们还可以把auto写到lambda表达式的形参中，这样就得到了一个泛型的lambda表达式，例如：
//    auto l = [](auto a1, auto a2) { return a1 + a2; };
//    auto retval = l(5, 5.0);

//typeid是C++标准的 typeof是GCC的
//    std::cout << typeid(int).name() << std::endl;

    return 0;
}
