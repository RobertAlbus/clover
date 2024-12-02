file(GLOB_RECURSE CLOVER_BENCHMARK_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.benchmark.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(clover_benchmark ${CLOVER_BENCHMARK_SOURCE_CPP})
target_include_directories(clover_benchmark PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(clover_benchmark PUBLIC benchmark::benchmark GTest::gtest libclover)
