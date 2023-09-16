########
# Clover: Composable Signal Graphs for Creative Sound Coding

cmake_minimum_required(VERSION 3.14)
project(Clover VERSION 2.1)
configure_file(includes/Config.h.in Config.h)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


########
# Clover variables

set(CLOVER_BUILD_DEMO ON)



########
# cmake variables

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_PARALLEL_LEVEL 8 CACHE STRING "Number of cores used for compilation")

# Enable SSE, AVX if the target architecture supports it
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    set(ENABLE_SSE OFF)
    set(ENABLE_AVX OFF)

    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
        set(ENABLE_SSE ON)
    endif()

    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(ENABLE_AVX ON)
    endif()

    if(ENABLE_SSE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse")
    endif()

    if(ENABLE_AVX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
    endif()
endif()


########
# external libraries

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

########
# internal libraries

add_library(${PROJECT_NAME} STATIC ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

add_library(${PROJECT_NAME}_Profiling STATIC ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME}_Profiling PRIVATE -Ofast -g)
target_link_libraries(${PROJECT_NAME}_Profiling PRIVATE profiler)
target_include_directories(${PROJECT_NAME}_Profiling
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
)
target_link_libraries(${PROJECT_NAME}_Profiling PUBLIC ${EXTRA_LIBS})

add_library(${PROJECT_NAME}_Strict STATIC EXCLUDE_FROM_ALL ${SOURCE_LIST_INCLUDES_CPP})
target_compile_options(${PROJECT_NAME}_Strict PRIVATE -Ofast)
target_include_directories(${PROJECT_NAME}_Strict
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
)
target_link_libraries(${PROJECT_NAME}_Strict PUBLIC ${EXTRA_LIBS})
target_compile_options(${PROJECT_NAME}_Strict PRIVATE
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


########
# libs, linking, etc

add_executable(${PROJECT_NAME}_Demo src/main.cpp)
target_link_libraries(${PROJECT_NAME}_Demo PUBLIC ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_Demo_Profiling src/main.cpp)
target_link_libraries(${PROJECT_NAME}_Demo_Profiling PUBLIC ${PROJECT_NAME}_Profiling)

add_custom_target(demo
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Clover_Demo
  DEPENDS CloverDemo
  USES_TERMINAL
)


function(print_all_targets DIR)
    get_property(TGTS DIRECTORY "${DIR}" PROPERTY BUILDSYSTEM_TARGETS)
    foreach(TGT IN LISTS TGTS)
        message(STATUS "Target: ${TGT}")
        # TODO: Do something about it
    endforeach()

    get_property(SUBDIRS DIRECTORY "${DIR}" PROPERTY SUBDIRECTORIES)
    foreach(SUBDIR IN LISTS SUBDIRS)
        print_all_targets("${SUBDIR}")
    endforeach()
endfunction()

print_all_targets(.)

include(commands/CMakeLists.commands.txt)
include(CMakeLists.tests.txt)