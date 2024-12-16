file(GLOB_RECURSE CLOVER_BENCHMARK_INVESTIGATE_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(benchmark_investigate ${CLOVER_BENCHMARK_INVESTIGATE_SOURCE_CPP})
target_include_directories(benchmark_investigate PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(benchmark_investigate PUBLIC benchmark::benchmark_main)
