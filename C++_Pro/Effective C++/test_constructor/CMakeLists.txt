cmake_minimum_required(VERSION 3.25)
project(test_constructor)

set(CMAKE_CXX_STANDARD 11)

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
    # 链接时需要加上的库
    # 查找Boost库，指定所需的组件（这里是thread）
    find_package(Boost REQUIRED COMPONENTS thread)
    # 如果找到Boost库，设置包含目录和链接库
    if(Boost_FOUND)
        message(STATUS "Boost found")
        include_directories(${Boost_INCLUDE_DIRS})
        if(DCMAKE_BUILD_TYPE STREQUAL "Release") # Release模式
            target_link_libraries(${my_executable_release}  ${Boost_LIBRARIES})
        else ()                                  # Debug模式
            target_link_libraries(${my_executable_debug}  ${Boost_LIBRARIES})
        endif()
    else ()
        message(FATAL_ERROR "Boost not found")
    endif()
endforeach()
