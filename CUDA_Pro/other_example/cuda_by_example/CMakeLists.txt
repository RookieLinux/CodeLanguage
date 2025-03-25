CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

set(CMAKE_CUDA_COMPILER "/usr/local/cuda-12.4/bin/nvcc")
set(CMAKE_CUDA_ARCHITECTURES 86)
project(cuda_by_example LANGUAGES CUDA CXX)

#Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
if(NOT OpenCV_FOUND)
	message(ERROR " OpenCV not found!")
endif(NOT OpenCV_FOUND)

#folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#outpath
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")


#CUDA
if(POLICY CMP0146)
	cmake_policy(SET CMP0146 OLD) # 使用旧的FindCUDA模块行为
endif()
find_package(CUDA 12.4 REQUIRED)
if (CUDA_FOUND)
	add_subdirectory(appendix_a)
	add_subdirectory(chapter03)
	add_subdirectory(chapter04)
	add_subdirectory(chapter05)
	add_subdirectory(chapter06)
	add_subdirectory(chapter07)
	add_subdirectory(chapter08)
	add_subdirectory(chapter09)
	add_subdirectory(chapter10)
else()
	message("CUDA not found!")
endif()



