CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

add_executable(enum_gpu enum_gpu.cu)
set_property(TARGET enum_gpu  PROPERTY FOLDER chapter03)
set_target_properties(enum_gpu PROPERTIES LINKER_LANGUAGE CXX)

add_executable(hello_world hello_world.cu)
set_property(TARGET hello_world  PROPERTY FOLDER chapter03)
set_target_properties(hello_world PROPERTIES LINKER_LANGUAGE CXX)

add_executable(set_gpu set_gpu.cu)
set_property(TARGET set_gpu  PROPERTY FOLDER chapter03)
set_target_properties(set_gpu PROPERTIES LINKER_LANGUAGE CXX)

add_executable(simple_device_call simple_device_call.cu)
set_property(TARGET simple_device_call  PROPERTY FOLDER chapter03)
set_target_properties(simple_device_call PROPERTIES LINKER_LANGUAGE CXX)

add_executable(simple_kernel_params simple_kernel_params.cu)
set_property(TARGET simple_kernel_params  PROPERTY FOLDER chapter03)
set_target_properties(simple_kernel_params PROPERTIES LINKER_LANGUAGE CXX)

add_executable(simple_kernel simple_kernel.cu)
set_property(TARGET simple_kernel  PROPERTY FOLDER chapter03)
set_target_properties(simple_kernel PROPERTIES LINKER_LANGUAGE CXX)
