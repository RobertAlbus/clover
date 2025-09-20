add_subdirectory(clover_lib)

if(CLOVER_BUILD_DEMO)
    add_subdirectory(demo)
endif()
if(CLOVER_BUILD_BENCHMARKS)
    add_subdirectory(clover_benchmark)
    add_subdirectory(benchmark_investigate)
endif()
