#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <iostream>

#include "clover_benchmark/intercept_reporter.hpp"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    benchmark::Initialize(&argc, argv);

    InterceptReporter reporter;
    benchmark::RunSpecifiedBenchmarks(&reporter);

    std::cout << "\n---------------------------------------------------------------------\n\n\n";

    return RUN_ALL_TESTS();
}
