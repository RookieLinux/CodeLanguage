//
// Created by rookie on 23-6-4.
//
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */


#if  1
struct multi_var
{
    volatile int threadAVar;
    volatile int threadBVar;
}g_multi_var __attribute__((aligned(64)));
#else
struct multi_var
{
    volatile int threadAVar __attribute__((aligned(64)));
    volatile int threadBVar __attribute__((aligned(64)));
}g_multi_var;
#endif

static const char *usage_str = "test cache\r\n";
static void usage()
{
    printf("%s", usage_str);
}

void test_cpu_performance(unsigned  int cpu)
{
    float f1 = 1.2;
    double db1 = 2.3;
    int i;
    int count = 0;
    int n = 0, n1 = 0;
    double d = 1;
    struct timeval a1, a2;
    gettimeofday(&a1, NULL);
    if(cpu == 0){
        count = 0;
        while (count++ <= 100){
            for (n = 1; n < 1024*1024; ++n) {
                g_multi_var.threadAVar += n;
            }
        }
    } else if(cpu == 1){
        count = 0;
        while (count++ <= 100){
            for (n = 1; n < 1024*1024; ++n) {
                g_multi_var.threadBVar += n;
            }
        }
    }
    gettimeofday(&a2, NULL);
    long time = 1000000*(a2.tv_sec-a1.tv_sec)+a2.tv_usec-a1.tv_usec;
    printf("cpu = %d,time = %ld exit\r\n", cpu, time);
}

static int set_cpu_affinity(unsigned int cpu)
{
    cpu_set_t cpuset;
    pid_t tid = syscall(SYS_gettid);
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    printf("before sched setaffinity cpu=%d\r\n", cpu);
    if(sched_setaffinity(tid, sizeof(cpuset), &cpuset) < 0){
        perror("sched setaffinity");
        printf("cpu %d\r\n", cpu);
        return -1;
    }
    printf("pid = %d, after sched setaffinity cpu=%d\r\n", tid, cpu);
    return 0;
}
static void* wast_cpu_body(void* arg)
{
    int i = 2;
    unsigned  int cpu = *(unsigned int *)arg;
    printf("wast_cpu_body in cpu=%d\r\n", cpu);
    set_cpu_affinity(cpu);
    test_cpu_performance(cpu);
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t *thread;
    int cpunum = 4;
    int ret = 0;
    int j = 0;
    int *icpu = NULL;
    pthread_attr_t *attr = NULL;
    void *p_arg = NULL;

    if(argc > 2){
        usage();
    }else if(argc == 2){
        cpunum = atoi(argv[1]);
        thread = malloc(sizeof(*thread) * cpunum);
        attr = malloc(sizeof(*attr) * cpunum);
        icpu = malloc(sizeof(*icpu) * cpunum);
        for (j = 0; j < cpunum; ++j) {
            ret = pthread_attr_init(&attr[j]);
//        ret |= pthread_attr_setdetachstate(&attr[j], PTHREAD_CREATE_DETACHED);
            icpu[j] = j;
            ret |= pthread_create(&thread[j], &attr[j], (void*(*)(void*))wast_cpu_body, &icpu[j]);
            printf("cpu index = %d\r\n", j);
        }
        for (j = 0; j < cpunum; ++j) {
            pthread_join(thread[j], NULL);
        }
        for (j = 0; j < cpunum; ++j) {
            pthread_attr_destroy(&attr[j]);
        }
        free(icpu);
        free(thread);
        free(attr);
    }

    return ret;
}