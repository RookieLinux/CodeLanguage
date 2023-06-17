#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;
using namespace std::string_literals; //C++14

int g_val;

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

struct mount_info {
    std::string device;
    std::string mount_point;
    std::string filesystem_type;
};

std::vector<mount_info> parseMountsFile(const std::string& filename) {
    std::vector<mount_info> mounts;

    std::ifstream mounts_file(filename);
    if (!mounts_file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return mounts;
    }

    std::string line;
    while (std::getline(mounts_file, line)) {
        std::istringstream iss(line);
        mount_info mountInfo;
        iss >> mountInfo.device >> mountInfo.mount_point >> mountInfo.filesystem_type;
        mounts.push_back(mountInfo);
    }

    return mounts;
}

std::vector<mount_info> parseMountsDev(const std::string& dev_name) {
    std::vector<mount_info> mounts;

    std::ifstream mounts_file("/proc/mounts");
    if (!mounts_file.is_open()) {
        std::cerr << "Failed to open " << "/proc/mounts" << std::endl;
        return mounts;
    }

    std::string line;
    while (std::getline(mounts_file, line)) {
        std::istringstream iss(line);
        mount_info mountInfo;
        iss >> mountInfo.device >> mountInfo.mount_point >> mountInfo.filesystem_type;
        // 使用strstr函数查找子串
        const char *ptr = strstr(mountInfo.device.c_str(), dev_name.c_str());
        if (ptr != NULL) {
            mounts.push_back(mountInfo);
            break;
        }
    }

    return mounts;
}


/*
 * @breif 判断磁盘设备是否是挂载的，如果已经挂载，输出块设备挂载信息
 * @param[in] 块设备路径 eg:/dev/nvme0n1
 * @param[out] 块设备挂载信息
 * @return 已经挂载返回true 反之false
*/
bool is_mounted(const std::string& dev_name, std::vector<mount_info>& info) {

    bool ret = false;
    std::ifstream mounts_file("/proc/mounts");
    if (!mounts_file.is_open()) {
        std::cerr << "Failed to open " << "/proc/mounts" << std::endl;
        return ret;
    }

    std::string line;
    while (std::getline(mounts_file, line)) {
        std::istringstream iss(line);
        mount_info mountInfo;
        iss >> mountInfo.device >> mountInfo.mount_point >> mountInfo.filesystem_type;
        // 使用strstr函数查找子串
        const char *ptr = strstr(mountInfo.device.c_str(), dev_name.c_str());
        if (ptr != NULL) {
            info.push_back(mountInfo);
            ret = true;
            break;
        }
    }

    return ret;
}


/*
 * @breif 判断磁盘设备是否是挂载的
 * @param[in] 块设备路径 eg:/dev/nvme0n1
 * @return 已经挂载返回true 反之false
*/
bool is_mounted(const std::string& dev_name) {

    bool ret = false;
    std::ifstream mounts_file("/proc/mounts");
    if (!mounts_file.is_open()) {
        std::cerr << "Failed to open " << "/proc/mounts" << std::endl;
        return ret;
    }

    std::string line;
    while (std::getline(mounts_file, line)) {
        std::istringstream iss(line);
        mount_info mountInfo;
        iss >> mountInfo.device >> mountInfo.mount_point >> mountInfo.filesystem_type;
        // 使用strstr函数查找子串
        const char *ptr = strstr(mountInfo.device.c_str(), dev_name.c_str());
        if (ptr != NULL) {
            ret = true;
            break;
        }
    }

    return ret;
}

int main() {
    int a = 10;
    std::string name = "test"s + "c++14"; //C++14

    const char *name_utf8 = u8"Rookie"; //等价"Rookie"
    const char16_t *name_utf16 = u"Rookie";
    const char32_t *name_utf32 = U"Rookie";
    const wchar_t *name_w = L"Rookie"; //windows下wchar_t 2字节；linux下 4字节
    std::cout << name << std::endl;
    std::cin.get();

    //测试获取函数
//    std::string str = "/mnt/nvme0n1/20240809103620/test.dat";
//    char delimiter = '/';
//    std::vector<std::string> tokens = split(str, delimiter);
//    // Print all tokens
//    std::cout << "All tokens:" << std::endl;
//    for (const auto& token : tokens) {
//        std::cout << token << std::endl;
//    }


    // 测试解析/proc
//    std::vector<MountInfo> mounts = parseMountsFile("/proc/mounts");
//    for (const auto& mount : mounts) {
//        std::cout << "Device: " << mount.device << std::endl;
//        std::cout << "Mount point: " << mount.mountPoint << std::endl;
//        std::cout << "Filesystem type: " << mount.filesystemType << std::endl;
//        std::cout << std::endl;
//    }

    std::vector<mount_info> mounts;
    bool is_mount = is_mounted("/dev/nvme0n1", mounts);
    std::cout << "dev size = " << mounts.size() << std::endl;
    for (const auto& mount : mounts) {
        std::cout << "Device: " << mount.device << std::endl;
        std::cout << "Mount point: " << mount.mount_point << std::endl;
        std::cout << "Filesystem type: " << mount.filesystem_type << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
