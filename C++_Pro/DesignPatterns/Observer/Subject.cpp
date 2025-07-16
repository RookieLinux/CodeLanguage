#include "Subject.h"
#include "Observer.h"
#include <iostream>
#include <list>
using namespace std;

Subject::Subject() {
    _obvs = new list<Observer *>;
}

Subject::~Subject() {
    if (_obvs) {
        delete _obvs;
        _obvs = nullptr;
    }
}

void Subject::Attach(Observer *obv) {
    _obvs->push_front(obv);
}

void Subject::Detach(Observer *obv) {
    if (obv)
        _obvs->remove(obv);
}

void Subject::Notify() {
    for (auto & _obv : *_obvs)
    {
        _obv->Update(this);
    }
}

ConcreteSubject::ConcreteSubject() {
    _st = '\0';
}

ConcreteSubject::~ConcreteSubject() = default;

void ConcreteSubject::SetState(const State &st) {
    _st = st;
}

State ConcreteSubject::GetState() {
    return _st;
}
