########
# Clover: Composable Signal Graphs for Creative Sound Coding

cmake_minimum_required(VERSION 3.14)
project(Clover VERSION 2.1)
configure_file(includes/Config.h.in Config.h)


########
# Clover variables

set(CLOVER_BUILD_DEMO ON)
set(CLOVER_BUILD_TESTS OFF)


########
# cmake variables

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -lprofiler")
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)


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

