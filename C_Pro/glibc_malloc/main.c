/* Per thread arena example. */

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void* threadFunc(void* arg) {

    printf("Before malloc in thread 1\n");
    getchar();
    char* addr = (char*) malloc(1000);
    printf("After malloc and before free in thread 1\n");
    getchar();
    free(addr);
    printf("After free in thread 1\n");
    getchar();

    return arg;
}


int main() {
    pthread_t t1;
    void* s;
    int ret;
    char* addr;

    printf("Welcome to per thread arena example::%d\n",getpid());
    printf("Before malloc in main thread\n");
    getchar();
    addr = (char*) malloc(1000);
    printf("After malloc and before free in main thread\n");
    getchar();
    free(addr);
    printf("After free in main thread\n");
    getchar();
    ret = pthread_create(&t1, NULL, threadFunc, NULL);
    if(ret)
    {
        printf("Thread creation error\n");
        return -1;
    }
    ret = pthread_join(t1, &s);
    if(ret)
    {
        printf("Thread join error\n");
        return -1;
    }
    return 0;
}

#else

#include <sys/mount.h>
#include <sys/statfs.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int main() {
    struct statfs buf;
//    const char* root_path = "/";  // 根文件系统路径
    const char* root_path = "/";  // 根文件系统路径

    if (statfs(root_path, &buf) != 0) {
        perror("statfs");
        return 1;
    }

    // 输出文件系统的总容量和可用容量
    printf("Total space: %lld MB\n",buf.f_frsize * buf.f_blocks / (1024 * 1024ull));
    printf("Available space: %lld MB\n",buf.f_frsize * buf.f_bfree / (1024 * 1024ull));
    return 0;
}

#endif