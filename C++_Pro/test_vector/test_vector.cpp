#include <iostream>
#include <vector>

using namespace std;
class Pos{
public:
    Pos(int x = 0, int y = 0, int z = 0);
    Pos(const Pos& p);
    friend  std::ostream& operator << (std::ostream& os, const Pos& p);
private:
    int m_x, m_y, m_z;
};

Pos::Pos(int x, int y, int z){
    m_x = x;
    m_y = y;
    m_z = z;
    std::cout << "create Pos" <<std::endl;
}
Pos::Pos(const Pos &p){
    m_x = p.m_x;
    m_y = p.m_y;
    m_z = p.m_z;
    std::cout << "copy Pos" <<std::endl;
}
std::ostream& operator << (std::ostream& os, const Pos& p){
    os << p.m_x << "," << p.m_y << "," << p.m_z;
    return os;
}

int main() {

    std::vector<Pos> pos; //默认是0个空间大小
    std::cout << "1. pos.capacity()="<< pos.capacity()
              << " pos.size()="<< pos.size() << std::endl;
//    pos.reserve(3); //设定空间大小
//    cout << "2. pos.capacity()="<< pos.capacity()
//              << " pos.size()="<< pos.size() << std::endl;

#if 1
    pos.push_back({1,2,3}); //{1,2,3}等价Pos(1,2,3)，会先在栈上创建，然后再拷贝到vector开辟的空间
    pos.push_back({4,5,6});
    pos.push_back({7,8,9});
#else
    pos.emplace_back(1,2,3);
    pos.emplace_back(4,5,6);
    pos.emplace_back(7,8,9);
#endif
    std::cout << "3. pos.capacity()="<< pos.capacity()
              << " pos.size()="<< pos.size() << std::endl;
    cin.get();
    return 0;
}
