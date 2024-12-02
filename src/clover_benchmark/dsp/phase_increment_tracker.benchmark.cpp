// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "benchmark/benchmark.h"
#include "gtest/gtest.h"

#include "clover/dsp/phase_increment_tracker.hpp"
#include "clover/float.hpp"

#include "clover_benchmark/intercept_reporter.hpp"
#include "clover_benchmark/util.hpp"

static void phase_increment_tracker_is_realtime() {
    auto pit = clover::dsp::phase_increment_tracker::for_freq(clover_bm::fs_48k, 100);
    for (int i = 0; i < clover_bm::samples_10s_48k; ++i) {
        pit.phase(static_cast<clover_float>(i % 6));
        pit.freq(100 + static_cast<clover_float>(i % 6) * 100);
        benchmark::DoNotOptimize(pit.phase());
        benchmark::DoNotOptimize(pit.phase_offset());
        benchmark::DoNotOptimize(pit.freq());
        pit.tick();
    }
}

static void BM_phase_increment_tracker(benchmark::State& state) {
    for (auto _ : state) {
        phase_increment_tracker_is_realtime();
    }
}

BENCHMARK(BM_phase_increment_tracker);
TEST(minimum_performance, BM_phase_increment_tracker) {
    auto run = InterceptReporter::run_map.at("BM_phase_increment_tracker");
    EXPECT_LE(run.real_accumulated_time, 0.005 * 1e9);
}

TEST(ideal_performance, BM_phase_increment_tracker) {
    auto run = InterceptReporter::run_map.at("BM_phase_increment_tracker");
    EXPECT_LE(run.real_accumulated_time, 0.0025 * 1e9);
}
