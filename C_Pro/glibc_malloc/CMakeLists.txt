cmake_minimum_required(VERSION 3.24)
project(glibc_malloc C)

set(CMAKE_C_STANDARD 11)

add_executable(glibc_malloc main.c)
