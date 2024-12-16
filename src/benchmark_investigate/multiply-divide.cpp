#include <ranges>

#include <benchmark/benchmark.h>

static void BM_multiply_float(benchmark::State& state) {
    float denominator_two   = 1.f / 2.f;
    float denominator_float = 1.f / 0.567f;

    for (auto _ : state) {
        for (const auto& x : std::views::iota(0, 1000)) {
            float a = static_cast<float>(x) * 2.f;
            float b = static_cast<float>(x) * 0.456f;
            benchmark::DoNotOptimize(a * denominator_two);
            benchmark::DoNotOptimize(b * denominator_two);
            benchmark::DoNotOptimize(a * denominator_float);
            benchmark::DoNotOptimize(b * denominator_float);
        }
    }
}

static void BM_divide_float(benchmark::State& state) {
    float denominator_two   = 2.f;
    float denominator_float = 0.567f;

    for (auto _ : state) {
        for (const auto& x : std::views::iota(0, 1000)) {
            float a = static_cast<float>(x) * 2.f;
            float b = static_cast<float>(x) * 0.456f;
            benchmark::DoNotOptimize(a / denominator_two);
            benchmark::DoNotOptimize(b / denominator_two);
            benchmark::DoNotOptimize(a / denominator_float);
            benchmark::DoNotOptimize(b / denominator_float);
        }
    }
}

static void BM_multiply_double(benchmark::State& state) {
    double denominator_two   = 1. / 2.;
    double denominator_float = 1. / 0.567;

    for (auto _ : state) {
        for (const auto& x : std::views::iota(0, 1000)) {
            double a = static_cast<double>(x) * 2.;
            double b = static_cast<double>(x) * 0.456;
            benchmark::DoNotOptimize(a * denominator_two);
            benchmark::DoNotOptimize(b * denominator_two);
            benchmark::DoNotOptimize(a * denominator_float);
            benchmark::DoNotOptimize(b * denominator_float);
        }
    }
}
static void BM_divide_double(benchmark::State& state) {
    double denominator_two   = 2.;
    double denominator_float = 0.567;

    for (auto _ : state) {
        for (const auto& x : std::views::iota(0, 1000)) {
            double a = static_cast<double>(x) * 2.;
            double b = static_cast<double>(x) * 0.456;
            benchmark::DoNotOptimize(a / denominator_two);
            benchmark::DoNotOptimize(b / denominator_two);
            benchmark::DoNotOptimize(a / denominator_float);
            benchmark::DoNotOptimize(b / denominator_float);
        }
    }
}

BENCHMARK(BM_multiply_float);
BENCHMARK(BM_divide_float);

BENCHMARK(BM_multiply_double);
BENCHMARK(BM_divide_double);
