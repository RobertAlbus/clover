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
