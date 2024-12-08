#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <iostream>

#include "clover_benchmark/intercept_reporter.hpp"

int main(int argc, char** argv) {
    // benchmark calls exit(0) after printing --help, so initialize it second
    testing::InitGoogleTest(&argc, argv);
    benchmark::Initialize(&argc, argv);

    if (benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;

    InterceptReporter reporter;
    benchmark::RunSpecifiedBenchmarks(&reporter);
    benchmark::Shutdown();

    std::cout << "\n---------------------------------------------------------------------\n\n\n";

    return RUN_ALL_TESTS();
}
