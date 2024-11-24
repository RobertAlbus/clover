add_subdirectory(clover_lib)

if(CLOVER_BUILD_BENCHMARKS)
    add_subdirectory(clover_benchmark)
endif()
if(CLOVER_BUILD_TESTS)
    add_subdirectory(clover_test)
endif()
