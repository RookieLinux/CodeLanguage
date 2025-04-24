cmake_minimum_required(VERSION 3.25)
project(test_kernel_struct C)

set(CMAKE_C_STANDARD 11)

add_executable(test_list common.h test_kernel_list.h test_kernel_list.c)
add_executable(test_hlist common.h test_kernel_hlist.h test_kernel_hlist.c)
