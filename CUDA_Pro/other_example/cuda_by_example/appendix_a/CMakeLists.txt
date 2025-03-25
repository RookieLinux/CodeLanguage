CMAKE_MINIMUM_REQUIRED(VERSION 3.18)


add_executable(hashtable_cpu hashtable_cpu.cu)
set_property(TARGET hashtable_cpu  PROPERTY FOLDER appendix_a)
set_target_properties(hashtable_cpu PROPERTIES LINKER_LANGUAGE CXX)

add_executable(hashtable_gpu hashtable_gpu.cu lock.h)
set_property(TARGET hashtable_gpu PROPERTY FOLDER appendix_a)
set_target_properties(hashtable_gpu PROPERTIES LINKER_LANGUAGE CXX)

add_executable(dot_0 dot.cu  lock.h)
set_property(TARGET dot_0 PROPERTY FOLDER appendix_a)
set_target_properties(dot_0 PROPERTIES LINKER_LANGUAGE CXX)
