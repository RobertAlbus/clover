#include <ranges>

#include <benchmark/benchmark.h>

// always checking the size of entire vector
static void BM_std_distance_whole_size(benchmark::State& state) {
    std::vector<float> data;
    data.resize(480000, 0);

    for (auto _ : state) {
        for (auto _ : std::views::iota(0, 480000)) {
            benchmark::DoNotOptimize(std::distance(data.begin(), data.end()));
        }
    }
}

// checking the distance between two points
static void BM_std_distance_two_points(benchmark::State& state) {
    std::vector<float> data;
    data.resize(480000, 0);

    for (auto _ : state) {
        for (auto i : std::views::iota(0, 480000)) {
            benchmark::DoNotOptimize(std::distance(data.begin() + i, data.end()));
        }
    }
}

BENCHMARK(BM_std_distance_whole_size);
BENCHMARK(BM_std_distance_two_points);

/*

Run on (20 X 5200 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x10)
  L1 Instruction 32 KiB (x10)
  L2 Unified 1280 KiB (x10)
  L3 Unified 24576 KiB (x1)
Load Average: 0.57, 0.40, 0.22
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_std_distance_whole_size      94356 ns        94290 ns         7552
BM_std_distance_two_points     121405 ns       121211 ns         5809


BM_std_distance_whole_size ~ 1/105,000x of real-time budget
BM_std_distance_two_points ~  1/82,000x of real-time budget

*/
