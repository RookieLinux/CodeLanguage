
cmake_minimum_required(VERSION 3.22)

project(StandardLib)

add_subdirectory(test_auto)
add_subdirectory(test_decltype)
add_subdirectory(test_placement-new)
add_subdirectory(test_smart_pointer)
add_subdirectory(test_sort)
add_subdirectory(test_string)
add_subdirectory(test_structured_bindings)
add_subdirectory(test_template)
add_subdirectory(test_unordermap)
add_subdirectory(test_vector)
add_subdirectory(test_virtual_destructor)

