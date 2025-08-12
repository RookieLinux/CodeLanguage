cmake_minimum_required(VERSION 3.16)

project(14-Storage VERSION 0.1 LANGUAGES CXX)

# 当前目录
set(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 需要忽略的目录列表
set(IGNORE_DIRS
    build
    .git
    cmake
    out
    tmp
)

# 找到当前目录下的所有一级子目录
file(GLOB children RELATIVE ${CURRENT_DIR} ${CURRENT_DIR}/*)
foreach(child ${children})
    # 判断是否是目录
    if(IS_DIRECTORY ${CURRENT_DIR}/${child})
        # 检查是否在忽略列表中
        list(FIND IGNORE_DIRS ${child} _ignore_index)
        if(_ignore_index EQUAL -1)
            # 检查是否存在 CMakeLists.txt
            if(EXISTS ${CURRENT_DIR}/${child}/CMakeLists.txt)
                message(STATUS "Adding subdirectory: ${child}")
                add_subdirectory(${child})
            else()
                message(STATUS "Skipping ${child} (no CMakeLists.txt found)")
            endif()
        else()
            message(STATUS "Skipping ignored directory: ${child}")
        endif()
    endif()
endforeach()




