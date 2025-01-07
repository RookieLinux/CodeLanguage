//
// Created by rookie on 23-12-19.
//

/*
 *   -----语法----
 *   C++11标准lambda表达式的语法非常简单，具体定义如下：
 *   [ captures ] ( params ) specifiers exception -> ret { body }
 *   语法规定lambda表达式如果存在说明符，那么形参列表不能省略。
 *
 *   例如：[x](int y)->int { return x * y; }
 *   [ captures ] —— 捕获列表，它可以捕获当前函数作用域的零个或多个变量，变量之间用逗号分隔。另外，捕获列表的捕获方式有两种：按值捕获和引用捕获
 *   ( params ) —— 可选参数列表，语法和普通函数的参数列表一样，在不需要参数的时候可以忽略参数列表。
 *   specifiers —— 可选限定符，C++11中可以用mutable，它允许我们在lambda表达式函数体内改变按值捕获的变量，或者调用非const的成员函数。
 *   exception —— 可选异常说明符，我们可以使用noexcept来指明lambda是否会抛出异常。
 *   ret —— 可选返回值类型，不同于普通函数，lambda表达式使用返回类型后置的语法来表示返回类型，如果没有返回值（void类型），可以忽略包括->在内的整个部分。
 *          另外，我们也可以在有返回值的情况下不指定返回类型，这时编译器会为我们推导出一个返回类型。对应到上面的例子是->int。
 *   { body } —— lambda表达式的函数体，这个部分和普通函数的函数体一样。对应例子中的{ return x * y; }。
 *
 *   -----作用域----
 *   标准还规定能捕获的变量必须是一个自动存储类型。简单来说就是非静态的局部变量、非全局变量。
 *
 *   -----捕获值和捕获引用-----
 *   捕获引用的语法与捕获值只有一个&的区别，要表达捕获引用我们只需要在捕获变量之前加上&，类似于取变量指针。
 *   只不过这里捕获的是引用而不是指针，在lambda表达式内可以直接使用变量名访问变量而不需解引用，比如：
 *   int main()
 *   {
 *      int x = 5, y = 8;
 *      auto foo = [&x, &y] { return x * y; };
 *   }
 *
 *   -----无状态lambda表达式-----
 *
 *   -----广义捕获-----
 *   C++14标准中定义了广义捕获，所谓广义捕获实际上是两种捕获方式，第一种称为简单捕获，这种捕获就是我们在前文中提到的捕获方法，即[identifier]、[&identifier]以及[this]等。
 *   第二种叫作初始化捕获，这种捕获方式是在C++14标准中引入的，它解决了简单捕获的一个重要问题，即只能捕获lambda表达式定义上下文的变量，而无法捕获表达式结果以及自定义捕获变量名，比如：
 *
*/
#include <iostream>
#include <vector>
#include <algorithm>

class A
{
public:
    void print()
    {
        std::cout << "class A" << std::endl;
    }
    void test()
    {
        auto foo = [this]
        {
            print();
            x = 5;
        };
        foo();
    }
private:
    int x;
};

int main()
{
    A a;
    a.test();

    std::vector<int> x = {1, 2, 3, 4, 5};
    std::cout << *std::find_if(x.cbegin(), x.cend(),[](int i) { return (i % 2) == 0; }) << std::endl;

    //广义捕获，C++14之前不支持
//    int x = 5;
//    auto foo = [x = x + 1]{ return x; };

    return 0;
}

//捕获全局、静态变量   GCC不会报错 而是报警告
//int x = 0;
//int main()
//{
//    int y = 0;
//    static int z = 0;
//    auto foo = [x, y, z] {};
//}