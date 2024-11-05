cmake_minimum_required(VERSION 3.25)

set(CMAKE_CUDA_COMPILER "/usr/local/cuda-11.8/bin/nvcc")
set(CMAKE_CUDA_ARCHITECTURES 86)
message(STATUS "CMAKE_CUDA_ARCHITECTURES is ${CMAKE_CUDA_ARCHITECTURES}")

project(test_helloWorld CUDA)
set(CMAKE_CUDA_STANDARD 17)

#检查是否成功找到CUDA
include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    message(STATUS "Find CUDA !")
else()
    message(FATAL_ERROR "Not found CUDA!")
endif()

add_executable(test_helloWorld test_helloWorld.cu)
set_target_properties(test_helloWorld PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
