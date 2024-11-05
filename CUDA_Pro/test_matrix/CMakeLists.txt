cmake_minimum_required(VERSION 3.22)

set(CMAKE_CUDA_COMPILER "/usr/local/cuda-11.8/bin/nvcc")
set(CMAKE_CUDA_ARCHITECTURES 86)
message(STATUS "CMAKE_CUDA_ARCHITECTURES is ${CMAKE_CUDA_ARCHITECTURES}")

project(test_matrix CUDA)
set(CMAKE_CUDA_STANDARD 14)

add_executable(test_matrix cudastart.h main.cu)

set_target_properties(test_matrix PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
