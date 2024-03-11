cmake_minimum_required(VERSION 3.25)
project(test_kernel_list C)

set(CMAKE_C_STANDARD 11)

add_executable(test_kernel_list list.h test_kernel_list.c)
