cmake_minimum_required(VERSION 3.25)
project(test_matrix CUDA)

set(CMAKE_CUDA_STANDARD 14)

add_executable(test_matrix main.cu)

set_target_properties(test_matrix PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
