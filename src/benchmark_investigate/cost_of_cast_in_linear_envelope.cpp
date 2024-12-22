#include <cmath>
#include <ranges>

#include <benchmark/benchmark.h>

int current_step_i    = 0;
float current_step_f  = 0;
float target_step_f   = 0;
float to_f            = 0;
float from_f          = 0;
double current_step_d = 0;
double target_step_d  = 0;
double to_d           = 0;
double from_d         = 0;

auto test_range = std::views::iota(0, 1000000);

void init_environment() {
    current_step_i = 0;
    current_step_f = 0;
    target_step_f  = 0;
    to_f           = 0;
    from_f         = 0;
    current_step_d = 0;
    target_step_d  = 0;
    to_d           = 0;
    from_d         = 0;
}

float increment_with_int() {
    if (static_cast<float>(current_step_i) >= target_step_f)
        return to_f;
    return std::lerp(from_f, to_f, static_cast<float>(current_step_i++) / target_step_f);
}
float increment_with_float() {
    if (current_step_f >= target_step_f)
        return to_f;
    return std::lerp(from_f, to_f, static_cast<float>(current_step_f++) / target_step_f);
}
double increment_with_double() {
    if (current_step_d >= target_step_d)
        return to_d;
    return std::lerp(from_d, to_d, static_cast<float>(current_step_d++) / target_step_d);
}

static void BM_increment_with_int(benchmark::State& state) {
    for (auto _ : state) {
        init_environment();
        for (auto _ : test_range) {
            benchmark::DoNotOptimize(increment_with_int());
        }
    }
}

static void BM_increment_with_float(benchmark::State& state) {
    for (auto _ : state) {
        init_environment();
        for (auto _ : test_range) {
            benchmark::DoNotOptimize(increment_with_float());
        }
    }
}

static void BM_increment_with_double(benchmark::State& state) {
    for (auto _ : state) {
        init_environment();
        for (auto _ : test_range) {
            benchmark::DoNotOptimize(increment_with_double());
        }
    }
}

BENCHMARK(BM_increment_with_int);
BENCHMARK(BM_increment_with_float);
BENCHMARK(BM_increment_with_double);
