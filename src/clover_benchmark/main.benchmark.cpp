// This file is part of Clover Audio Framework, Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "benchmark/benchmark.h"

#include "clover/example.hpp"

static void BM_example(benchmark::State& state) {
    long n = state.range(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(clover::example());
    }
}

BENCHMARK(BM_example)->Arg(10)->Arg(15)->Arg(20);

BENCHMARK_MAIN();
