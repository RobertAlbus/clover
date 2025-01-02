cmake_minimum_required(VERSION 3.30)

project(clover)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_PARALLEL_LEVEL 8 STRING)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

message("\n======== clover ========")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CLOVER_IS_SUBPROJECT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
set(CLOVER_IS_MAIN_PROJECT TRUE)

if(NOT CLOVER_IS_SUBPROJECT)
    set(CLOVER_IS_MAIN_PROJECT FALSE)
endif()

option(CLOVER_BUILD_TESTS      "Build tests"      ${CLOVER_IS_MAIN_PROJECT})
option(CLOVER_BUILD_BENCHMARKS "Build benchmarks" ${CLOVER_IS_MAIN_PROJECT})
option(CLOVER_BUILD_DEMO       "Build demo"       ${CLOVER_IS_MAIN_PROJECT})

if(${CLOVER_BUILD_TESTS})
    enable_testing()
endif()

add_subdirectory(cmake)
add_subdirectory(src)

# Add this to your CMakeLists.txt or a debug script
get_property(all_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)

function(getAllSubdirs dir dirs)
    message("called with dir (${dir}) and dirs (${dirs})")
    set(_dirs "")
    # get subdirectories for dir
    get_property(subdirs DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    # iterate any found subdirectories
    foreach(subdir ${subdirs})
        # append each sub directory
        list(APPEND _dirs ${subdir})
        set(${dirs} _dirs PARENT_SCOPE)
        getAllSubdirs(${subdir} ${_dirs})
    endforeach()
endfunction()

set(dirs ".")
getAllSubDirs(. ${dirs})
message("all dirs " ${dirs})