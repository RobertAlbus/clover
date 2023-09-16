#--------------------------------
# Clover: Composable Signal Graphs for Creative Sound Coding

cmake_minimum_required(VERSION 3.14)
project(Clover VERSION 2.1)
configure_file(includes/Config.h.in Config.h)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


#--------------------------------
# cmake variables

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_PARALLEL_LEVEL 8 CACHE STRING "Number of cores used for compilation")


#--------------------------------
# dependencies

link_libraries(m)
link_libraries(pthread)

# PortAudio
add_subdirectory(dependencies/portaudio)
list(APPEND EXTRA_LIBS PortAudio)

# wave
option(wave_enable_tests "Build Unit tests" OFF)
add_subdirectory(dependencies/wave)
list(APPEND EXTRA_LIBS wave)

# rtmidi
set(RTMIDI_TARGETNAME_UNINSTALL "uninstall_rtmidi" CACHE STRING "Name of 'uninstall' build target")
add_subdirectory(dependencies/rtmidi)
list(APPEND EXTRA_LIBS rtmidi)

set_target_properties(PortAudio PROPERTIES QUIET TRUE)
set_target_properties(wave PROPERTIES QUIET TRUE)
set_target_properties(rtmidi PROPERTIES QUIET TRUE)


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
file(GLOB_RECURSE SOURCE_LIST_TEST_CC
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/test/*.cc
)


#--------------------------------
# SIMD detect/enable

list(APPEND COMMON_ARCH_FLAGS)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        list(APPEND COMMON_ARCH_FLAGS "-msse")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        list(APPEND COMMON_ARCH_FLAGS "-mavx")
    endif()
endif()


#--------------------------------
# libraries

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME} PRIVATE -Ofast ${COMMON_ARCH_FLAGS})
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

add_library(${PROJECT_NAME}_Profiling STATIC ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME}_Profiling PRIVATE -Ofast -g ${COMMON_ARCH_FLAGS})
target_link_libraries(${PROJECT_NAME}_Profiling PRIVATE profiler)
target_include_directories(${PROJECT_NAME}_Profiling
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
)
target_link_libraries(${PROJECT_NAME}_Profiling PUBLIC ${EXTRA_LIBS})

add_library(${PROJECT_NAME}_Strict STATIC EXCLUDE_FROM_ALL ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME}_Strict PRIVATE -Ofast ${COMMON_ARCH_FLAGS})
target_include_directories(${PROJECT_NAME}_Strict
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
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

add_executable(${PROJECT_NAME}_Demo_Profiling src/main.cpp)
target_link_libraries(${PROJECT_NAME}_Demo_Profiling PUBLIC ${PROJECT_NAME}_Profiling)


#--------------------------------
# commands

include(commands/CMakeLists.commands.txt)


#--------------------------------
# tests

include(CMakeLists.tests.txt)
