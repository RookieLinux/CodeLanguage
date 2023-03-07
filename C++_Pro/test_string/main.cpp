#include <iostream>
#include <string>

using namespace std;
using namespace std::string_literals; //C++14

int g_val;

int main() {
    int a = 10;
    std::string name = "test"s + "c++14"; //C++14

    const char *name_utf8 = u8"Rookie"; //等价"Rookie"
    const char16_t *name_utf16 = u"Rookie";
    const char32_t *name_utf32 = U"Rookie";
    const wchar_t *name_w = L"Rookie"; //windows下wchar_t 2字节；linux下 4字节
    std::cout << name << std::endl;
    std::cin.get();
    return 0;
}
