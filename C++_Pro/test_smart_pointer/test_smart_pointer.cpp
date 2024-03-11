#include <iostream>
#include <memory> //智能指针头文件

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
            std::unique_ptr<Entity> entity = std::make_unique<Entity>(); //异常安全
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

    std::cout << "Hello, World!" << std::endl;
    std::cin.get();
    return 0;
}
