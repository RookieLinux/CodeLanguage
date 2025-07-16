cmake_minimum_required(VERSION 3.22)
project(Observer)

set(CMAKE_CXX_STANDARD 17)
# 设置编译器优化等级为O3
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# 关闭返回值优化(RVO)
# set(CMAKE_CXX_FLAGS "-fno-elide-constructors")

include_directories("${PROJECT_BINARY_DIR}")

# 发现当前目录下的所有.cpp/.h文件
file(GLOB files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

if(CMAKE_BUILD_TYPE STREQUAL "Release") # Release模式
    set(my_executable_release ${PROJECT_NAME}Release)
    add_executable(${my_executable_release} ${files})
else ()                                  # Debug模式
    set(my_executable_debug ${PROJECT_NAME}Debug)
    add_executable(${my_executable_debug} ${files})
endif()