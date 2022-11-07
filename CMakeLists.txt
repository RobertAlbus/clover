########
# Clover: Composable Signal Graphs for Creative Sound Coding

set(version 3.10)
cmake_minimum_required(VERSION ${version})
project(Clover VERSION 1.0)
configure_file(includes/config.h.in config.h)


########
# Clover variables

set(CLOVER_BUILD_DEMO ON)


########
# cmake variables

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
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

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${EXTRA_LIBS})

########
# libs, linking, etc



target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_BINARY_DIR}
        ${EXTRA_INCLUDES}
        includes
        src
)

if(CLOVER_BUILD_DEMO)
    add_executable(${PROJECT_NAME}Demo src/main.cpp)
    target_link_libraries(${PROJECT_NAME}Demo PUBLIC ${PROJECT_NAME})
endif()
