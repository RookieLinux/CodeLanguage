#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

std::tuple<int, int> return_multiple_values()
{
    return std::make_tuple(11, 7);
}

struct BindTest {
    int a = 42;
    std::string b = "hello structured binding";
};


int main() {
    int x = 0, y = 0;
    std::tie(x, y) = return_multiple_values();
    std::cout << "x=" << x << " y=" << y << std::endl;

    auto[ax, by] = return_multiple_values();
    std::cout << "ax=" << ax << " by=" << by << std::endl;


    BindTest bt1;
    //ax1 by1是绑定到bt1副本(匿名对象)的名称
    auto[ax1, by1] = bt1;
    std::cout << "ax1=" << ax1 << " by1=" << by1
              << " &ax1=" << &ax1 << " &by1=" << &by1
              << " &bt1.a=" << &bt1.a << " &bt1.b=" << &bt1.b<<std::endl;
    //ax2 by2是绑定到bt1的名称，并且可以通过这种方式更改bt1
    auto& [ax2, by2] = bt1;
    std::cout << "ax2=" << ax2 << " by2=" << by2
              << " &ax2=" << &ax2 << " &by2=" << &by2
              << " &bt1.a=" << &bt1.a << " &bt1.b=" << &bt1.b<<std::endl;

    //结构化绑定遍历
    std::vector<BindTest> bt_vct{ {11, "hello"}, {7, "c++"}, {42,"world"} };
    for (const auto& [bt_a, bt_b] : bt_vct) {
        std::cout << "bt_a=" << bt_a << " bt_b=" << bt_b << std::endl;
    }

    //结构化绑定的别名还有一个限制：无法在同一个作用域中重复使用。这一点和变量声明是一样的，比如：
    //auto[ax1, by1] = bt1; //编译报错

    return 0;
}
