#if 0

#include <stdio.h>
#include "src/log4c/log4c.h"

int main() {
    int rc = 0;
    log4c_category_t* mycat = NULL;

    if (log4c_init()){
        printf("log4c_init() failed");
        rc = 1;
    }else{
        mycat = log4c_category_get("six13log.log");
        if(mycat) {
            log4c_category_log(mycat, LOG4C_PRIORITY_ERROR, "Hello World!");
        }
        /* Explicitly call the log4c cleanup routine */
        if (log4c_fini()){
            printf("log4c_fini() failed");
        }
    }
    return 0;
}

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <linux/nvme_ioctl.h>
//#include <linux/nvme.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define SYS_BLOCK_PATH "/sys/block"

int main() {
    DIR *dir;
    struct dirent *entry;

    // 打开/sys/block目录
    dir = opendir(SYS_BLOCK_PATH);
    if (dir == NULL) {
        perror("Unable to open directory");
        return EXIT_FAILURE;
    }

    // 遍历/sys/block下的所有文件
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_LNK && strncmp(entry->d_name, ".", 1) != 0) {
            char dev_path[256];
            char dev_name[256];
            char dev_name_buf[256];
            char symlink[256];

            // 构造设备路径
            snprintf(dev_path, sizeof(dev_path), "%s/%s", SYS_BLOCK_PATH, entry->d_name);
            printf("dev_path: %s\n", dev_path);
        }
    }

    closedir(dir);

    int fd;
    struct nvme_passthru_cmd cmd = {0};
    char smart_log[1024] = {0};

    // 打开NVMe设备文件
    fd = open("/dev/nvme0", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open NVMe device");
        exit(EXIT_FAILURE);
    }

    // 构建SMART命令
//    cmd.opcode = nvme_admin_cmd_smart;
    cmd.nsid = 1;  // 命名空间编号，通常为1
    cmd.addr = (unsigned long)&smart_log;
    cmd.data_len = sizeof(smart_log);
    cmd.cdw10 = 1;  // 表示读取SMART数据

    // 发送SMART命令
    if (ioctl(fd, NVME_IOCTL_ADMIN_CMD, &cmd) < 0) {
        perror("NVMe SMART command ioctl failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 解析SMART数据
    printf("NVMe SMART data:\n");
    printf("%s\n", smart_log);
    // 还可以读取其他SMART数据字段

    // 关闭设备文件
    close(fd);

    return EXIT_SUCCESS;
}

#endif