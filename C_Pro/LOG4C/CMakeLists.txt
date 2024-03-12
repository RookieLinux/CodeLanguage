cmake_minimum_required(VERSION 3.25)
project(LOG4C C)

set(CMAKE_C_STANDARD 11)

# 增加宏定义
add_compile_definitions(HAVE_CONFIG_H)
# main.c路径
set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
# 头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/log4c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/sd)
# 源文件路径
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/log4c SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/sd SRC_LIST2)
# 添加源文件
set(SRC_ALL_LIST ${SRC_LIST}  ${SRC_LIST2} ${MAIN_DIR})
add_executable(LOG4C ${SRC_ALL_LIST})
## 链接第三方库(使用第三方expat解析xml文件时开启，使用自带xml解析不需要开启，目前的源码使用自带xml解析)
#find_package(EXPAT REQUIRED)
#if(EXPAT_FOUND)
#    include_directories(${EXPAT_INCLUDE_DIRS})
#    target_link_libraries(LOG4C ${EXPAT_LIBRARIES})
#endif(EXPAT_FOUND)