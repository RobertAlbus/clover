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

if(NOT PROFILING_MODE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -lprofiler")
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

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
add_subdirectory(dependencies/portaudio EXCLUDE_FROM_ALL)
list(APPEND EXTRA_LIBS PortAudio)

# wave
option(wave_enable_tests "Build Unit tests" OFF)
add_subdirectory(dependencies/wave)
list(APPEND EXTRA_LIBS wave)

# rtmidi
set(RTMIDI_TARGETNAME_UNINSTALL "uninstall_rtmidi" CACHE STRING "Name of 'uninstall' build target")
add_subdirectory(dependencies/rtmidi)
list(APPEND EXTRA_LIBS rtmidi)


########
# internal libraries

add_library(${PROJECT_NAME} STATIC 
    ${CMAKE_CURRENT_LIST_DIR}/includes/Base/CloverBase.cpp

    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Envelope/EnvelopeStruct.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Envelope/ADSR.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Envelope/BasicEnvelope.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Envelope/DC.cpp

    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/StepSequencer/PatternSettable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/StepSequencer/StepSequencer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/StepSequencer/lib/Pitchable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/StepSequencer/lib/Triggerable.cpp

    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Stereo/Difference.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Stereo/MidSideBalance.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Stereo/Pan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Stereo/Sum.cpp

    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Wavetable/WavetableOsc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/NodeSimplex/Wavetable/WavetableOscStereo.cpp

    ${CMAKE_CURRENT_LIST_DIR}/includes/Util/Calc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/Util/MusicTime.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/Util/SampleClock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/includes/Util/Wavetable.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

########
# tests
if(CLOVER_BUILD_TESTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/CTest.cmake)
endif()

########
# libs, linking, etc

if(CLOVER_BUILD_DEMO)
    add_executable(${PROJECT_NAME}Demo src/main.cpp)
    target_link_libraries(${PROJECT_NAME}Demo PUBLIC ${PROJECT_NAME})
endif()

