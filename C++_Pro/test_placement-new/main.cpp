#include <iostream>
#include <string>
#include <new>
using namespace std;

/*
 * 使用定位new创建对象，显式调用析构函数是必须的，这是析构函数必须被显式调用的少数情形之一！，
 * 另有一点！！！析构函数的调用必须与对象的构造顺序相反！切记！！！
 * */

const int BUF = 512;
class JustTesting {
private:
    string words;
    int number;
public:
    JustTesting(const string& s = "Just Testing", int n = 0) {
        words = s;
        number = n;
        cout << words << " constructed" << endl;
    }
    ~JustTesting() { cout << words << " destroyed!" << endl; }
    void Show()const { cout << words << " , " << number << endl; }
};

int main() {
    char* buffer = new char[BUF];//常规new在堆上申请空间
    cout << "\nMemory block address:\n" << "buffer: " << (void*)buffer
         << "\n****************************************\n";

    JustTesting* pc1, * pc2;
    pc1 = new (buffer) JustTesting;//定位new
    pc2 = new JustTesting("Heap1", 20);//常规new

    cout << "\nMemory contents:" << endl;
    cout << pc1 << ": ";
    pc1->Show();
    cout << pc2 << ": ";
    pc2->Show();

    cout << "----------------------------------------\n";

    JustTesting* pc3, * pc4;
    pc3 = new (buffer)JustTesting("Bad Idea", 6);//定位new
    pc4 = new JustTesting("Heap2", 10);//常规new

    cout << "\nMemory contents:" << endl;
    cout << pc3 << ": ";
    pc3->Show();
    cout << pc4 << ": ";
    pc4->Show();

    delete pc2;//释放pc2申请的空间
    delete pc4;//释放pc4申请的空间
    delete[] buffer;//释放buffer指向的空间,但是这里并没有调用其析构函数
    //pc3->~JustTesting(); //需要显式调用构造函数
    //pc1->~JustTesting();
    cout << "Done!" << endl;
    return 0;
}
/*
 * 定位new的使用格式：
 * 1.new (place_address) type
 * 2.new (palce_address) type (initializer_list)
 * 用法1与用法2的区别在于对象是否需要初始化，其中place_address必须是一个指针，initializer_list是type类型的初始化列表。
 * */

/*由例程代码及结果分析可以看出：
 * 1.使用定位new创建的对象的地址与内存缓冲区地址一致，说明定位new并没有申请新空间，而构造函数的调用说明定位new的确调用了构造函数。
 * 2.在使用delete回收空间时，可以发现并未回收pc1与pc3，其原因在于pc1与pc3指向的对象位于内存缓冲区，该空间并不是定位new申请，
 *   而是常规new申请的，因此我们需要delete[]回收内存缓冲区，而不是delete pc1与delete pc3
 * 3.pc1与pc3一致，说明第一个对象被第三个覆盖！显然，如果类动态地为其成员分配内存，这将引发问题！，
 *   所以，当我们使用定位new创建对象必须自己保证不会覆盖任何不想丢失的数据！，就这个例程而言，避免覆盖，最简单的做法如下：
 *   pc1 = new (buffer) JustTesting;
 *   pc3 = new (buffer + sizeof(JustTesting)) JustTesting("Better Idea!",6);
 * 4.delete[] buffer并未引发对象的析构！，虽然对象1及3的空间被回收，但对象1与3并未析构！这一点将时刻提醒我们使用定位new需要自己显式调用析构函数，完成对象的析构！，
 *   但该析构并不能通过delete pc1或delete pc3实现！（因为delete与定位new不能配合使用！，否则会引发运行时错误！），只能通过显式析构，如下：
 *   pc3->~JustTesting();
 *   pc1->~JustTesting();
*/
