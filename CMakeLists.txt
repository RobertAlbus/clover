# THIS IS SUPER NAIVE.
# eventually this project should be broken
# into sublibraries for modular compilation. 
# for now this is fine.



cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

# set the project name
project(Clover)

link_libraries(asound)
link_libraries(m)
link_libraries(pthread)
link_libraries(portaudio)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(${PROJECT_NAME} 
    src/main.cpp

    src/interface/interface.cpp    

    src/time/musicTime.cpp
    src/time/sampleClock.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        src/buffer
        src/calc
        src/interface
        src/graph
        src/nodes
        src/sine
        src/time
        src/wavetable
)