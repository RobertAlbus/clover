file(GLOB_RECURSE CPP_BOILERPLATE_BENCHMARK_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(cpp_boilerplate_benchmark ${CPP_BOILERPLATE_BENCHMARK_SOURCE_CPP})
target_include_directories(cpp_boilerplate_benchmark PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(cpp_boilerplate_benchmark PUBLIC benchmark::benchmark libcpp_boilerplate)