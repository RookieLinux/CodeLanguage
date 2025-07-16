cmake_minimum_required(VERSION 3.22)
project(test_structured_bindings)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_structured_bindings
        test_structured_bindings.cpp)

add_executable(test_structured_bindings1
        lang/customer1.hpp
        lang/structbind1.hpp
        lang/structbind1.cpp)

add_executable(test_structured_bindings2
        lang/customer2.hpp
        lang/structbind2.hpp
        lang/structbind2.cpp)
