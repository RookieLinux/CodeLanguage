//
// Created by rookie on 25-07-16.
//
#include "Subject.h"
#include "Observer.h"
#include <iostream>
using namespace  std;

int main()
{
    auto* sub = new ConcreteSubject();
    Observer* o1 = new ConcreteObserverA(sub);
    Observer* o2 = new ConcreteObserverB(sub);
    sub->SetState("old");
    sub->Notify();
    std::cout <<"\r\n";
    sub->SetState("new");
    sub->Notify(); //也可以由Observer调用
    delete o1;
    delete o2;
    delete sub;

    return 0;
}
