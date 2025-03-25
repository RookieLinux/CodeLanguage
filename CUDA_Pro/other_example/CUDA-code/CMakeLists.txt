CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

if(POLICY CMP0146)
	cmake_policy(SET CMP0146 OLD) # 使用旧的FindCUDA模块行为
endif()

set(CMAKE_CUDA_ARCHITECTURES 86)
set(CMAKE_CUDA_COMPILER "/usr/local/cuda-11.8/bin/nvcc")
set(CMAKE_C_COMPILER "/usr/bin/gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -ccbin ${CMAKE_CXX_COMPILER} -Xcompiler -fPIC" CACHE STRING "" FORCE)
project(CUDA-code)

#folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#outpath
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")  

#CUDA
find_package(CUDA 11.8 REQUIRED)
if (CUDA_FOUND)
	add_subdirectory(1.2lesson)
	add_subdirectory(2.1lesson)
	add_subdirectory(2.2lesson)
	add_subdirectory(2.3lesson)
	add_subdirectory(3.1lesson)
	#[[
add_subdirectory(2.6lesson)
add_subdirectory(3.2lesson)
add_subdirectory(3.3lesson)
add_subdirectory(3.4lesson)
add_subdirectory(3.5lesson)
add_subdirectory(4.3lesson)
add_subdirectory(4.4lesson)
add_subdirectory(4.5lesson)
add_subdirectory(4.6lesson)
add_subdirectory(4.7lesson)
add_subdirectory(4.8lesson)
add_subdirectory(4.10lesson)
#]]
else()
	message("CUDA not found!")
endif()
