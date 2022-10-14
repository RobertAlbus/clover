# THIS IS SUPER NAIVE.
# eventually this project should be broken
# into sublibraries for modular compilation. 
# for now this is fine.


set(version 3.10)
cmake_minimum_required(VERSION ${version})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

# set the project name
project(Clover VERSION 1.0)

# link_libraries(asound)
link_libraries(m)
link_libraries(pthread)

# PortAudio
add_subdirectory(dependencies/portaudio EXCLUDE_FROM_ALL)
list(APPEND EXTRA_LIBS PortAudio)

# wave
option(wave_enable_tests "Build Unit tests" OFF)
add_subdirectory(dependencies/wave)
list(APPEND EXTRA_LIBS wave)

configure_file(src/config.h.in config.h)

add_subdirectory(src/util)
list(APPEND EXTRA_LIBS CloverUtil)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/util")

add_subdirectory(src/graph)
list(APPEND EXTRA_LIBS Graph)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/graph")

list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/IO")

add_subdirectory(src/NodeSimplex)
list(APPEND EXTRA_LIBS NodeSimplex)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/NodeSimplex")


add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/IO/interface.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})


target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_BINARY_DIR}
        ${EXTRA_INCLUDES}
        src
)