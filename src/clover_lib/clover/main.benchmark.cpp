#include <benchmark/benchmark.h>
#include <cstring>
#include <gtest/gtest.h>
#include <iostream>

#include "clover/clover_benchmark/intercept_reporter.hpp"

int main(int argc, char** argv) {
    bool has_bm_filter = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strncmp(argv[i], "--benchmark_filter", 18) == 0) {
            has_bm_filter = true;
            break;
        }
    }

    // benchmark calls exit(0) after printing --help, so initialize it second
    testing::InitGoogleTest(&argc, argv);
    benchmark::Initialize(&argc, argv);

    if (benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;

    InterceptReporter reporter;
    benchmark::RunSpecifiedBenchmarks(&reporter);
    benchmark::Shutdown();

    if (has_bm_filter) {
        std::cout << "\nskipping performance assertions (benchmark filter active)\n";
        return 0;
    }

    std::cout << "\n---------------------------------------------------------------------\n\n\n";

    return RUN_ALL_TESTS();
}
