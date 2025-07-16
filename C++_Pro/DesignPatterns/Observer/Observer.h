#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "Subject.h"
#include <string>
using namespace std;
typedef string State;

class Observer {
public:
    virtual ~Observer();
    virtual void Update(Subject* sub) = 0;
    virtual void PrintInfo() = 0;
protected:
    Observer();
    State _st;
private:
};


class ConcreteObserverA:public Observer
{
public:
    virtual Subject* GetSubject();
    explicit ConcreteObserverA(Subject* sub);
    ~ConcreteObserverA() override;
    void Update(Subject* sub) override;
    void PrintInfo() override;
protected:
private:
    Subject* _sub;
};


class ConcreteObserverB:public Observer
{
public:
    virtual Subject* GetSubject();
    explicit ConcreteObserverB(Subject* sub);
    ~ConcreteObserverB() override;
    void Update(Subject* sub) override;
    void PrintInfo() override;
protected:
private:
    Subject* _sub;
};

#endif