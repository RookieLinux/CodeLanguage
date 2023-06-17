#include <iostream>
#include <algorithm>
#include <functional> //内置函数
#include <string>
// 参考
// https://zh.cppreference.com/w/cpp/algorithm/sort
// https://blog.csdn.net/qq_19887221/article/details/124380534

struct student
{
    std::string name;
    int math;
    int eng;
};

bool comp(student a, student b){    // 先按数学成绩降序，再看英语成绩降序
    if(a.math == b.math)
        return a.eng > b.eng;
    return a.math > b.math;
}

bool ascend(int a, int b){
    return a < b;           // true时 a 在 b 前面，即升序
}
bool descend(int a, int b){
    return a > b;           // true时 a 在 b 前面，即降序
}

int main() {

    int a[]={45,12,34,77,90,11,2,4,5,55};

//    //1、sort函数第三个参数采用默认从小到大
//    std::sort(a, a + 10);
//    for(int i = 0; i < 10; i++)
//        std::cout << a[i] <<" ";
//    std::cout << "---" <<"\n";


//    //2、参数三采用内置函数
//    sort(a, a + 10, std::greater<int>());   // 降序
//    for(int i = 0; i < 10; i++)
//        std::cout << a[i] <<" ";
//    std::cout << "---" <<"\n";
//    sort(a, a + 10, std::less<int>());      // 升序
//    for(int i = 0; i < 10; i++)
//        std::cout << a[i] <<" ";
//    std::cout << "---" <<"\n";

//    //3、自定义函数
//    std::sort(a, a + 10, descend);
//    for(int i = 0; i < 10; i++)
//        std::cout << a[i] <<" ";
//    std::cout << "---" <<"\n";
//    std::sort(a, a + 10, ascend);
//    for(int i = 0; i < 10; i++)
//        std::cout << a[i] <<" ";
//    std::cout << "---" <<"\n";
//
    //4、lambda函数
    std::sort(a, a + 10, [](int a, int b){
          return a < b; // 升序
    });
    for(int i = 0; i < 10; i++)
        std::cout << a[i] <<" ";
    std::cout << "---" <<"\n";


    student students[] = {
            {"aa", 80, 90},     // cc 100 60
            {"bb", 90, 80},     // dd 90 100
            {"cc", 100 ,60},    // bb 90 80
            {"dd", 90, 100}     // aa 80 90
    };
    std::sort(students, students + 4, comp);
    for(auto stu : students){
        std::cout << stu.name << " " << stu.math << " " << stu.eng << "\n";
    }

    return 0;
}
