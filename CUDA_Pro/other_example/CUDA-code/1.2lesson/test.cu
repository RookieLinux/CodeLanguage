#include <stdio.h>

__global__ void hello_from_gpu()
{
    printf("Hello World from the the GPU, grid%d th%d\n", blockIdx.x, threadIdx.x);
}


int main(void)
{
    hello_from_gpu<<<4, 4>>>();
    cudaDeviceSynchronize();

    return 0;
}
