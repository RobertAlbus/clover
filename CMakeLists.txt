#--------------------------------
# Clover
# 
# Composable Signal Graphs for Creative Sound Coding
#--------------------------------

include(CMakeLists.vcpkg.txt)
cmake_minimum_required(VERSION 3.14)
project(Clover)


#--------------------------------
# cmake variables

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_PARALLEL_LEVEL 8 STRING)


#--------------------------------
# SIMD detect/enable

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        list(APPEND CLOVER_COMPILE_FLAGS "-msse")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        list(APPEND CLOVER_COMPILE_FLAGS "-mavx")
    endif()
endif()


#--------------------------------
# libraries

list(APPEND CLOVER_INCLUDE_DIRECTORIES
    ${PROJECT_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include
)
include(CMakeLists.Clover.txt)


#--------------------------------

include(commands/CMakeLists.txt)
include(test/CMakeLists.txt)
include(examples/CMakeLists.txt)

