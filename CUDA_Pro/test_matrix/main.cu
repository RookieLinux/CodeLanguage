#include <stdio.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "cudastart.h"


double cpuSecond()
{
    struct timeval tp;
    gettimeofday(&tp,NULL);
    return((double)tp.tv_sec+(double)tp.tv_usec*1e-6);

}

void initialData(float* ip,int size)
{
    time_t t;
    srand((unsigned )time(&t));
    for(int i=0;i<size;i++)
    {
        ip[i]=(float)(rand()&0xffff)/1000.0f;
    }
}

void initDevice(int devNum)
{
    int dev = devNum;
    cudaDeviceProp deviceProp;
    CHECK(cudaGetDeviceProperties(&deviceProp,dev));
    printf("Using device %d: %s\n",dev,deviceProp.name);
    printf("warpSize: %d\n", deviceProp.warpSize);
    CHECK(cudaSetDevice(dev));

}

void checkResult(float * hostRef,float * gpuRef,const int N)
{
    double epsilon=1.0E-8;
    for(int i=0;i<N;i++)
    {
        if(abs(hostRef[i]-gpuRef[i])>epsilon)
        {
            printf("Results don\'t match!\n");
            printf("%f(hostRef[%d] )!= %f(gpuRef[%d])\n",hostRef[i],i,gpuRef[i],i);
            return;
        }
    }
    printf("Check result success!\n");
}

//CPU对照组，用于对比加速比
void sumMatrix2DonCPU(float * MatA,float * MatB,float * MatC,int nx,int ny)
{
    float* a = MatA;
    float* b = MatB;
    float* c = MatC;
    for(int j=0; j<ny; j++)
    {
        for(int i=0; i<nx; i++)
        {
            c[i] = a[i]+b[i];
        }
        c += nx;
        b += nx;
        a += nx;
    }
}

//核函数，每一个线程计算矩阵中的一个元素。
__global__ void sumMatrix(float * MatA,float * MatB,float * MatC,int nx,int ny)
{
    /*
     * gridDim: 这是一个三维变量，用于表示一个网格的维度。CUDA中的线程结构可以被组织成一个三维的网格，每个维度可以有多个块（blocks）。
     *          gridDim.x 表示网格在 x 维度上的块数，gridDim.y 表示网格在 y 维度上的块数，gridDim.z 表示网格在 z 维度上的块数。
     * blockDim: 这也是一个三维变量，用于表示一个块的维度。每个块都可以包含多个线程。
     *          blockDim.x 表示块在 x 维度上的线程数，blockDim.y 表示块在 y 维度上的线程数，blockDim.z 表示块在 z 维度上的线程数。
     * blockIdx: 这是一个三维变量，用于表示当前线程所属的块（block）在整个网格（grid）中的索引。
     *          blockIdx.x 表示当前线程所属的块在 x 维度上的索引，blockIdx.y 表示在 y 维度上的索引，blockIdx.z 表示在 z 维度上的索引。
     * threadIdx: 这也是一个三维变量，用于表示当前线程在其所属的块中的索引。
     *          每个块都可以包含多个线程，而 threadIdx.x 表示当前线程在 x 维度上的索引，threadIdx.y 表示在 y 维度上的索引，threadIdx.z 表示在 z 维度上的索引。
     * */
    int ix = threadIdx.x+blockDim.x*blockIdx.x;
    int iy = threadIdx.y+blockDim.y*blockIdx.y;
    int idx = ix+iy*ny;
    if (ix<nx && iy<ny)
    {
        MatC[idx] = MatA[idx]+MatB[idx];
    }
}

//主函数
int main(int argc,char** argv)
{
    //设备初始化
    printf("strating...\n");
    initDevice(0);

    //输入二维矩阵，4096*4096，单精度浮点型。
    int nx = 1<<12;
    int ny = 1<<12;
    int nBytes = nx*ny*sizeof(float);

    //Malloc，开辟主机内存
    float* A_host = (float*)malloc(nBytes);
    float* B_host = (float*)malloc(nBytes);
    float* C_host = (float*)malloc(nBytes);
    float* C_from_gpu = (float*)malloc(nBytes);
    initialData(A_host, nx*ny);
    initialData(B_host, nx*ny);

    //cudaMalloc，开辟设备内存
    float* A_dev = NULL;
    float* B_dev = NULL;
    float* C_dev = NULL;
    CHECK(cudaMalloc((void**)&A_dev, nBytes));
    CHECK(cudaMalloc((void**)&B_dev, nBytes));
    CHECK(cudaMalloc((void**)&C_dev, nBytes));

    //输入数据从主机内存拷贝到设备内存
    CHECK(cudaMemcpy(A_dev, A_host, nBytes, cudaMemcpyHostToDevice));
    CHECK(cudaMemcpy(B_dev, B_host, nBytes, cudaMemcpyHostToDevice));

    //二维线程块，32×32
    dim3 block(32, 32);
    //二维线程网格，128×128
    dim3 grid((nx-1)/block.x+1, (ny-1)/block.y+1);

    //测试GPU执行时间
    double gpuStart = cpuSecond();
    printf("before sumMatrix\n");
    //将核函数放在线程网格中执行 (在调用内核函数的时候，会在<<< >>>内设置两个参数，分别代表线程网格的维度和线程块的维度。)
    sumMatrix<<<grid,block>>>(A_dev, B_dev, C_dev, nx, ny);
    printf("after sumMatrix\n");
    CHECK(cudaDeviceSynchronize());
    double gpuTime = cpuSecond() - gpuStart;
    printf("GPU Execution Time: %f sec\n", gpuTime);

    //在CPU上完成相同的任务
    cudaMemcpy(C_from_gpu, C_dev, nBytes, cudaMemcpyDeviceToHost);
    double cpuStart=cpuSecond();
    sumMatrix2DonCPU(A_host, B_host, C_host, nx, ny);
    double cpuTime = cpuSecond() - cpuStart;
    printf("CPU Execution Time: %f sec\n", cpuTime);

    //检查GPU与CPU计算结果是否相同
    CHECK(cudaMemcpy(C_from_gpu, C_dev, nBytes, cudaMemcpyDeviceToHost));
    checkResult(C_host, C_from_gpu, nx*ny);

    //释放内存
    cudaFree(A_dev);
    cudaFree(B_dev);
    cudaFree(C_dev);
    free(A_host);
    free(B_host);
    free(C_host);
    free(C_from_gpu);
    cudaDeviceReset();
    return 0;
}