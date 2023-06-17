cmake_minimum_required(VERSION 3.24)
project(C_Pro C)

#set(CMAKE_C_STANDARD 11)

ADD_SUBDIRECTORY(glibc_malloc)
ADD_SUBDIRECTORY(test_cache)
ADD_SUBDIRECTORY(test_posix_timer)