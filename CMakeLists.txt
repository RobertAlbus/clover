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
set(CMAKE_BUILD_PARALLEL_LEVEL 8 CACHE STRING "Number of cores used for compilation")


#--------------------------------
# dependencies

link_libraries(m)
link_libraries(pthread)

# PortAudio
find_package(portaudio CONFIG REQUIRED)
list(APPEND EXTRA_LIBS portaudio)

# libsndfile
find_package(SndFile CONFIG REQUIRED)
list(APPEND EXTRA_LIBS SndFile::sndfile)

# rtmidi
find_package(rtmidi CONFIG REQUIRED)
list(APPEND EXTRA_LIBS RtMidi::rtmidi)

#--------------------------------
# source globs

file(GLOB_RECURSE SOURCE_LIST_INCLUDES_H
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/includes/*.h
)
file(GLOB_RECURSE SOURCE_LIST_INCLUDES_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/includes/*.cpp
)
file(GLOB_RECURSE SOURCE_LIST_SRC_H
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.h
)
file(GLOB_RECURSE SOURCE_LIST_SRC_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE SOURCE_LIST_TEST_FUNCTIONAL_CC
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/test/functional/*.cc
)
file(GLOB_RECURSE SOURCE_LIST_TEST_HELPERS_H
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/test/*.h
)
file(GLOB_RECURSE SOURCE_LIST_TEST_PERFORMANCE_CC
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/test/performance/*.cc
)


#--------------------------------
# SIMD detect/enable

include(CheckCXXCompilerFlag)
function(enable_SIMD_flag flag)
    check_cxx_compiler_flag("${flag}" flag_supported)
    if(flag_supported)
        list(APPEND COMMON_ARCH_FLAGS "${flag}")
    endif()
endfunction()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    enable_SIMD_flag("-msse")
    enable_SIMD_flag("-msse2")
    enable_SIMD_flag("-msse3")
    enable_SIMD_flag("-mssse3")
    enable_SIMD_flag("-msse4.1")
    enable_SIMD_flag("-msse4.2")
    enable_SIMD_flag("-mavx")
    enable_SIMD_flag("-mavx2")
endif()


#--------------------------------
# libraries

list(APPEND CLOVER_INCLUDE_DIRECTORIES
    ${PROJECT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include
    ${CMAKE_CURRENT_LIST_DIR}/includes
)


# Clover - optimized
add_library(${PROJECT_NAME} STATIC ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME} PRIVATE -Ofast ${COMMON_ARCH_FLAGS})
target_include_directories(${PROJECT_NAME}
PUBLIC ${CLOVER_INCLUDE_DIRECTORIES}
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})


# Clover - profiling
add_library(${PROJECT_NAME}_Profiling STATIC EXCLUDE_FROM_ALL ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME}_Profiling PRIVATE -Ofast -g ${COMMON_ARCH_FLAGS})
target_link_libraries(${PROJECT_NAME}_Profiling PRIVATE profiler)
target_include_directories(${PROJECT_NAME}_Profiling
PUBLIC ${CLOVER_INCLUDE_DIRECTORIES}
)
target_link_libraries(${PROJECT_NAME}_Profiling PUBLIC ${EXTRA_LIBS})


# Clover - strict
add_library(${PROJECT_NAME}_Strict STATIC EXCLUDE_FROM_ALL ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME}_Strict PRIVATE -Ofast ${COMMON_ARCH_FLAGS})
target_include_directories(${PROJECT_NAME}_Strict
    PUBLIC ${CLOVER_INCLUDE_DIRECTORIES}
)
target_link_libraries(${PROJECT_NAME}_Strict PUBLIC ${EXTRA_LIBS})
target_compile_options(${PROJECT_NAME}_Strict PRIVATE ${COMMON_ARCH_FLAGS}
    -Werror # enable this to interpret warnings as errors
    -pedantic
    -Wall
    -Wextra
    -Wcast-align
    -Wcast-qual
    -Wctor-dtor-privacy
    -Wdisabled-optimization
    -Wformat=2
    -Winit-self
    -Wlogical-op
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Wnoexcept
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wshadow
    -Wsign-conversion
    -Wsign-promo
    -Wstrict-null-sentinel
    -Wstrict-overflow=5
    -Wswitch-default
    -Wundef
    -Wno-unused
)


#--------------------------------
# executable demos

add_executable(${PROJECT_NAME}_Demo src/main.cpp)
target_link_libraries(${PROJECT_NAME}_Demo PUBLIC ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_Demo_Profiling EXCLUDE_FROM_ALL src/main.cpp)
target_link_libraries(${PROJECT_NAME}_Demo_Profiling PUBLIC ${PROJECT_NAME}_Profiling)

#--------------------------------
# commands

include(commands/CMakeLists.commands.txt)


#--------------------------------
# tests

include(test/CMakeLists.tests.txt)
