cmake_minimum_required(VERSION 3.14)

if(CLOVER_EXAMPLES_CMAKE_INCLUDE_GUARD)
  return()
endif()
set(CLOVER_EXAMPLES_CMAKE_INCLUDE_GUARD TRUE INTERNAL "Inclusion guard for examples/CMakeLists.txt")

# add_executable(Clover_Demo ${CMAKE_CURRENT_LIST_DIR}/demo/main.cpp)
# target_link_libraries(Clover_Demo PUBLIC Clover)

# add_executable(Clover_Demo_Profiling EXCLUDE_FROM_ALL ${CMAKE_CURRENT_LIST_DIR}/demo/main.cpp)
# target_link_libraries(Clover_Demo_Profiling PUBLIC Clover_Profiling)