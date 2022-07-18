# THIS IS SUPER NAIVE.
# eventually this project should be broken
# into sublibraries for modular compilation. 
# for now this is fine.



cmake_minimum_required(VERSION 3.10)

# set the project name
project(TryPort)

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
    src/sine/sine.cpp
    src/time/time.h
    src/time/time.cpp
    
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        src/sine
        src/interface
)