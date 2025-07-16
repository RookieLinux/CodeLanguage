#include <iostream>
#include <memory> //智能指针头文件
#include <vector>
#include <string>
#include <algorithm> // 包含 std::remove

class Entity
{
public:
    Entity()
    {
        std::cout <<"Created Entity"<<std::endl;
    }
    ~Entity()
    {
        std::cout <<"Destoryed Entity"<<std::endl;
    }
    void Print()
    {

    }
};

//shared_ptr使用问题例子,循环引用导致内存泄漏  https://blog.csdn.net/tjcwt2011/article/details/137120026
//将ConcreteObserver类中的subject_成员变量改为weak_ptr，这样就打破循环引用，不会导致内存无法正确释放了。
class IObserver {
public:
    virtual void update(const std::string& msg) = 0;
};

class Subject {
public:
    void attach(const std::shared_ptr<IObserver>& observer) {
        observers_.emplace_back(observer);
    }
    void detach(const std::shared_ptr<IObserver>& observer) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }
    void notify(const std::string& msg) {
        for (auto& observer : observers_) {
            observer->update(msg);
        }
    }
private:
    std::vector<std::shared_ptr<IObserver>> observers_;
};

class ConcreteObserver : public IObserver {
public:
    explicit ConcreteObserver(const std::shared_ptr<Subject>& subject) : subject_(subject) {}
    void update(const std::string& msg) override {
        std::cout << "ConcreteObserver " << msg<< std::endl;
    }
private:
#if 0
    std::shared_ptr<Subject> subject_;
#else
    std::weak_ptr<Subject> subject_; //解决循环引用问题
#endif
};


//智能指针应用----单例模式
class Singleton {
public:
    static std::shared_ptr<Singleton> getInstance() {
        std::shared_ptr<Singleton> instance = m_instance.lock();
        if (!instance) {
            instance.reset(new Singleton());
            m_instance = instance;
        }
        return instance;
    }
private:
    Singleton() {}
    static std::weak_ptr<Singleton> m_instance;
};
std::weak_ptr<Singleton> Singleton::m_instance;


int main() {

    {
#if 1
        std::shared_ptr<Entity> e0; //拷贝会增加引用计数
#else
        std::weak_ptr<Entity> e0; //拷贝不会增加引用计数
#endif
        {
            #if 1
            /*
             * unique_ptr不可以复制
             */
            std::unique_ptr<Entity> entity = std::make_unique<Entity>(); //异常安全 c++14才开始有make_unique
//            std::unique_ptr<Entity> entity(new Entity);
            entity->Print();
            #else
            /*
             * shared_ptr可以复制
             */
            std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>(); //提高效率
//            std::shared_ptr<Entity> sharedEntity(new Entity);
            e0 = sharedEntity;
            #endif
        }

    }

    std::cout << "循环引用" << std::endl;
    std::shared_ptr<Subject> subject = std::make_shared<Subject>();
    std::shared_ptr<IObserver> observer = std::make_shared<ConcreteObserver>(subject);
    subject->attach(observer);
    subject->notify("update");

    std::cout << "测试结束" << std::endl;
    return 0;
}
