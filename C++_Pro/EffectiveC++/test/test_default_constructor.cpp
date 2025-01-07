#include <iostream>

template<class T>
class DOG {
public:
    DOG(std::string& name, const T& value);
    // 这里我们只声明构造函数，不声明 copy 赋值运算符函数
private:
    std::string& name;    // 引用成员对象
    const T value;		  // 常量成员对象
};

int main() {
    std::cout << "测试默认构造函数" << std::endl;

    std::string newDog("Persephone");
    std::string oldDog("Satch");
    DOG<int> p(newDog, 2);          // 作者当时养的狗狗
    DOG<int> s(oldDog, 36);         // 作者之前养的已经去世的狗狗
    /*
     * 如果某个未指定 copy 构造函数或 copy 赋值运算符的类内存在引用成员对象或常量成员对象，需要执行类对象的 copy 操作时，编译器会拒绝编译。
     * 因为自动生成的 copy 构造函数或 copy 赋值运算符无法处理对引用成员对象和常量成员对象的赋值操作。
     *
     * 如果某个类的基类将 copy 构造函数 和 copy 赋值运算符函数声明为 private，那么该类中也不会由编译器自动生成这两个函数，
     * 编译器认为它没办法处理 copy 操作时，调用基类 copy 方法的操作。
     * */
    p = s;                          // 编译器对这条代码无能为力
    return 0;
}
