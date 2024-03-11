#include <iostream>

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


int main() {

    Base* base = new Base();
    delete base;
    std::cout << "--------------------------\n";
    Derived* derived = new Derived();
    delete derived;
    std::cout << "--------------------------\n";
    Base* base1 = new Derived();
    delete base1; //这种情况下只会执行基类的析构函数，所以引入虚析构函数

    return 0;
}

