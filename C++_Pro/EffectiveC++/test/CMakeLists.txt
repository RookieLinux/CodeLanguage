cmake_minimum_required(VERSION 3.22)
project(test)

set(CMAKE_CXX_STANDARD 17)

# 设置编译器优化等级为O3
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# 关闭返回值优化(RVO)
set(CMAKE_CXX_FLAGS "-fno-elide-constructors")

include_directories("${PROJECT_BINARY_DIR}")

# 发现当前目录下的所有.cpp文件
file(GLOB mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(cppfile IN LISTS mains)
    message(${cppfile})
    # 获取对应文件名不包含路径与后缀
    get_filename_component(my_executable ${cppfile} NAME_WLE)

    if(DCMAKE_BUILD_TYPE STREQUAL "Release") # Release模式
        set(my_executable_release ${my_executable}_release)
        add_executable(${my_executable_release} ${cppfile})
    else ()                                  # Debug模式
        set(my_executable_debug ${my_executable}_debug)
        add_executable(${my_executable_debug} ${cppfile})
    endif()
endforeach()
