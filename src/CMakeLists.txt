add_subdirectory(cpp_boilerplate_lib)

if(CPP_BOILERPLATE_BUILD_BENCHMARKS)
    add_subdirectory(cpp_boilerplate_benchmark)
endif()
if(CPP_BOILERPLATE_BUILD_TESTS)
    add_subdirectory(cpp_boilerplate_test)
endif()
