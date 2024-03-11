cmake_minimum_required(VERSION 3.25)
project(Melon C)

set(CMAKE_C_STANDARD 11)

# main.c路径
set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Melon.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
# 追加(拼接)
set(SRC_ALL_LIST ${SRC_LIST} ${MAIN_DIR})

add_executable(Melon ${SRC_ALL_LIST})
