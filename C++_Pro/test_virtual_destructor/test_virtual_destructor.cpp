#include <iostream>

/*
 * 所以析构函数要实现多态。But，析构函数名字天生不一样，怎么实现多态？
 * 实际上，析构函数被编译器全部换成了Destructor，所以我们加上virtual就可以。
 * 只要父类的析构函数用virtual修饰，无论子类是否有virtual，都构成析构。
 * 这也解释了为什么子类不写virtual可以构成重写，因为编译器怕你忘记析构。
 */
class Base{
public:
    Base(){
        std::cout << "Base Constructor\n";
    }
#if 0
    ~Base(){
        std::cout << "Base Destructor\n";
    }
#else
    virtual ~Base(){
        std::cout << "Base Destructor\n";
    }
#endif
};

class Derived:public Base{
public:
    Derived(){
        std::cout << "Derived Constructor\n";
    }
    ~Derived(){
        std::cout << "Derived Destructor\n";
    }
private:
    int * m_array;
};
/*
 * 协变：子类的虚函数和父类的虚函数的返回值可以不同，也能构成重载。但需要子类的返回值是一个子类的指针或者引用，父类的返回值是一个父类的指针或者引用，且返回值代表的两个类也成继承关系。这个叫做协变。
 */

class Person //成人
{
public:
    virtual Person* fun()
    {
        std::cout << "全价票\n"; //成人票全价
        return this;
    }
};
class Student : public Person //学生
{
public:
    virtual Student* fun() //子类完成对父类虚函数的重写
    {
        std::cout << "半价票\n";//学生票半价
        return this;
    }
};
void BuyTicket(Person* p)
{
    p->fun();
}


class A
{
public:
    virtual void fun(int val = 0)//父类虚函数
    {
        std::cout <<"A->val = "<< val << "\n";
    }
    void Fun()
    {
        fun();//传过来一个子类指针调用fun()
    }
};
class B : public A //B继承A
{
public:
    virtual void fun(int val = 1)//子类虚函数
    {
        std::cout << "B->val = " << val << "\n";
    }
};
class Person2
{
public:
    virtual A* fun()//返回A类指针
    {
        std::cout << "Person2::fun()\n";
        return nullptr;
    }
};
class Student2 : Person2
{
public:
    //返回B类指针，虽然返回值不同，也构成重写
    virtual B* fun()//子类重写父类虚函数
    {
        std::cout << "Student2::fun()\n";
        return nullptr;
    }
};

int main() {

    Base* base = new Base();
    delete base;
    std::cout << "--------------------------\n";
    Derived* derived = new Derived();
    delete derived;
    std::cout << "--------------------------\n";
    Base* base1 = new Derived();
    delete base1; //这种情况下只会执行基类的析构函数，所以引入虚析构函数

    std::cout << "--------------测试协变------------\n";
    //协变
    Student st;
    Person p;
    BuyTicket(&st);//子类对象切片过去
    BuyTicket(&p);//父类对象传地址

    Student2 st2;
    Person2 p2;
    Person2* ptr = &p2;
    ptr->fun();
    Person2* ptr2 = reinterpret_cast<Person2*>(&st2);
    ptr2->fun();

    std::cout << "--------------接口继承和实现继承------------\n";
    //普通函数的继承就是实现继承，虚函数的继承就是接口继承
    B b;
    A* a = &b;
    a->Fun();

    return 0;
}

