cmake_minimum_required(VERSION 3.25)
project(test_log4c C)

set(CMAKE_C_STANDARD 11)

# log4c
set(3RDLIB_PATH /usr/local/mylog4c)
list(APPEND 3RD_LIBS_INCLUDE ${3RDLIB_PATH}/include)
list(APPEND 3RD_LIBS_INCLUDE ${3RDLIB_PATH}/include/log4c)
include_directories(${3RD_LIBS_INCLUDE})
link_directories(${3RDLIB_PATH}/lib)
link_libraries(log4c)
add_executable(test_log4c test_log4c.c)
