cmake_minimum_required(VERSION 3.22)
project(test_cmake)

set(CMAKE_CXX_STANDARD 17)
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" GIT_SEMVER "1.7.2")
string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" SEMVER_SPLITED "${GIT_SEMVER}")
set(MAJOR_VERSION ${CMAKE_MATCH_1})
set(MINOR_VERSION ${CMAKE_MATCH_2})
set(PATCH_VERSION ${CMAKE_MATCH_3})
message(STATUS "Current semver: major: ${MAJOR_VERSION}, minor: ${MINOR_VERSION}, patch: ${PATCH_VERSION}, GIT_SEMVER: ${GIT_SEMVER}, SEMVER_SPLITED: ${SEMVER_SPLITED}")

#导入Qt相关依赖包
set(CMAKE_PREFIX_PATH /opt/Qt/Qt5.15.2/5.15.2/gcc_64)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Qml)

set(QT_SDK_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../..")
message(STATUS "1. QT_SDK_DIR: ${QT_SDK_DIR}")
cmake_path(SET QT_SDK_DIR NORMALIZE ${QT_SDK_DIR})
message(STATUS "2. QT_SDK_DIR: ${QT_SDK_DIR}")
message(STATUS "3. CMAKE_SOURCE_DIR：${CMAKE_SOURCE_DIR}")
message(STATUS "4. CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "5. CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "6. CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(test_cmake main.cpp
        Monster.cpp
        Monster.h)


# ----------------------------------------TEST----------------------------------------------
function(prepend OUT_VAR PREFIX) #Arguments 3, 4, etc. are items to prepend to
    set(TEMP "")
    foreach(ITEM ${ARGN})
        set(TEMP "${TEMP} ${PREFIX}${ITEM}")
    endforeach()
    set(${OUT_VAR} "${TEMP}" PARENT_SCOPE)
endfunction()

function(prependlist OUT_VAR PREFIX) #Arguments 3, 4, etc. are items to prepend to
    set(TEMP "")
    foreach(ITEM ${ARGN})
        set(TEMP "${TEMP};${PREFIX}${ITEM}")
    endforeach()
    set(${OUT_VAR} "${TEMP}" PARENT_SCOPE)
endfunction()

set(common_includes
    bhc.hpp
    math.hpp
    platform.hpp
    structs.hpp
)

set(common_source
    mode/arr.cpp
    mode/arr.hpp
)
prependlist(common_includes "${CMAKE_SOURCE_DIR}/include/bhc/" ${common_includes})
prependlist(common_source "${CMAKE_SOURCE_DIR}/src/" ${common_source})

message(STATUS "common_includes: ${common_includes}")
message(STATUS "common_source: ${common_source}")


function(get_compute_for_gpu OUT_VAR GPU_NAME)
    set(GPU_DATABASE "30:GTX 770,GTX 760,GT 740,GTX 690,GTX 680,GTX 670,GTX 660 Ti,GTX 660,GTX 650 Ti BOOST,GTX 650 Ti,GTX 650;35:GTX Titan Z,GTX Titan Black,GTX Titan,GTX 780 Ti,GTX 780;50:GTX 750 Ti,GTX 750;52:GTX Titan X,GTX 980 Ti,GTX 980,GTX 970,GTX 960,GTX 950;61:TITAN Xp,Titan X,GTX 1080 Ti,GTX 1080,GTX 1070 Ti,GTX 1070,GTX 1070 with Max-Q Design,GTX 1060,GTX 1050 Ti,GTX 1050;70:TITAN V;75:TITAN RTX,RTX 2080 Ti,RTX 2080 Super,RTX 2080,RTX 2070 Super,RTX 2070,RTX 2060 Super,RTX 2060,GTX 1660 Ti,GTX 1660 Super,GTX 1660,GTX 1650 Super,GTX 1650;86:RTX A5500 Laptop GPU,RTX 3090 Ti,RTX 3090,RTX 3080 Ti,RTX 3080,RTX 3070 Ti,RTX 3070,RTX 3060 Ti,RTX 3060,RTX 3050 Ti,RTX 3050;89:RTX A6000,RTX 4090,RTX 4080,RTX 4070 Ti,RTX 4060 Laptop GPU")
    foreach(COMPUTE_ROW ${GPU_DATABASE})
        message(STATUS "COMPUTE_ROW=" ${COMPUTE_ROW})
        string(REGEX REPLACE ":.*" "" COMPUTE_CAPABILITY ${COMPUTE_ROW})
        string(REGEX REPLACE ".*:" "" GPU_LIST ${COMPUTE_ROW})
        string(REGEX REPLACE "," ";" GPU_LIST ${GPU_LIST})
        if(${GPU_NAME} IN_LIST GPU_LIST)
            set(${OUT_VAR} ${COMPUTE_CAPABILITY} PARENT_SCOPE)
            return()
        endif()
    endforeach()
    set(${OUT_VAR} "Unknown" PARENT_SCOPE)
    message(SEND_ERROR "CUDA compute capability of GPU " ${GPU_NAME} " unknown!")
endfunction()


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(NVIDIA_SMI_COMMAND "nvidia-smi.exe")
    set(NVIDIA_SMI_FAILURE_MSG "Could not find nvidia-smi.exe. Please add the folder containing it to PATH or set CUDA_ARCH_OVERRIDE in CMake.")
else()
    set(NVIDIA_SMI_COMMAND "nvidia-smi")
    set(NVIDIA_SMI_FAILURE_MSG "nvidia-smi failed! Make sure the NVIDIA driver is installed and loaded!")
endif()
execute_process(COMMAND ${NVIDIA_SMI_COMMAND} "-L"
        RESULT_VARIABLE __res OUTPUT_VARIABLE __out
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
if(__res EQUAL 0)
    string(REGEX REPLACE "\n" ";" __out ${__out}) #Make into list
    set(ALL_COMPUTES "")
    set(WARN_MULTI_ARCH 0)
    foreach(GPU_LINE ${__out})
        string(REGEX REPLACE "GPU [0-9]: (.*) \\(UUID: .*" "\\1" GPU_NAME ${GPU_LINE})
        string(REGEX REPLACE "GeForce " "" GPU_NAME ${GPU_NAME})
        string(REGEX REPLACE "N[Vv][Ii][Dd][Ii][Aa] " "" GPU_NAME ${GPU_NAME})
        string(REGEX REPLACE " Laptop GPU" "" GPU_NAME ${GPU_NAME})
        get_compute_for_gpu(GPU_COMPUTE ${GPU_NAME})
        message(STATUS "GPU found: " ${GPU_NAME} ", compute " ${GPU_COMPUTE})
    endforeach()
else()
    message(FATAL_ERROR "${NVIDIA_SMI_FAILURE_MSG}")
endif()

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE PACKAGE_GIT_VERSION
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "${PACKAGE_GIT_VERSION}")