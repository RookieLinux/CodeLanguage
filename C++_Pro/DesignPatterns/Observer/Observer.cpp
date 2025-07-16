#include "Observer.h"
#include <iostream>
using namespace std;

Observer::Observer() {
    _st = '\0';
}

Observer::~Observer() = default;


ConcreteObserverA::ConcreteObserverA(Subject *sub) {
    _sub = sub;
    if (_sub != nullptr) {
        _sub->Attach(this);
    }
}

ConcreteObserverA::~ConcreteObserverA() {
    if (_sub != nullptr) {
        std::cout<<"~ConcreteObserverA"<<endl;
        _sub->Detach(this);
    }
}

Subject *ConcreteObserverA::GetSubject() {
    return _sub;
}

void ConcreteObserverA::Update(Subject *sub) {
    _st = sub->GetState();
    PrintInfo();
}

void ConcreteObserverA::PrintInfo() {
    cout<<"ConcreteObserverA _st is "<<_st<<endl;
}


ConcreteObserverB::ConcreteObserverB(Subject *sub) {
    _sub = sub;
    if (_sub != nullptr) {
        _sub->Attach(this);
    }
}

ConcreteObserverB::~ConcreteObserverB() {
    if (_sub != nullptr) {
        std::cout<<"~ConcreteObserverB"<<endl;
        _sub->Detach(this);
    }
}

Subject *ConcreteObserverB::GetSubject() {
    return _sub;
}

void ConcreteObserverB::Update(Subject *sub) {
    _st = sub->GetState();
    PrintInfo();
}

void ConcreteObserverB::PrintInfo() {
    cout<<"ConcreteObserverB _st is "<<_st<<endl;
}
