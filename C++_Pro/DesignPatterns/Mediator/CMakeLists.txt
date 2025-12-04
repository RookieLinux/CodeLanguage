cmake_minimum_required(VERSION 3.10)
project(mediator_mpmc_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mediator_demo
    main.cpp
)

target_include_directories(mediator_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

