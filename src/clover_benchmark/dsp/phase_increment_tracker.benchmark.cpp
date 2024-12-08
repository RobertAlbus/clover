// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "benchmark/benchmark.h"
#include "gtest/gtest.h"

#include "clover/dsp/phase_increment_tracker.hpp"
#include "clover/float.hpp"

#include "clover_benchmark/intercept_reporter.hpp"
#include "clover_benchmark/util.hpp"

static void BM_phase_increment_tracker(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto pit = clover::dsp::phase_increment_tracker::for_freq(clover_bm::fs_48k, 100);
        state.ResumeTiming();

        for (int i = 0; i < clover_bm::samples_10s_48k; ++i) {
            pit.phase(static_cast<clover_float>(i % 6));
            pit.freq(100 + static_cast<clover_float>(i % 6) * 100);
            benchmark::DoNotOptimize(pit.phase());
            benchmark::DoNotOptimize(pit.phase_offset());
            benchmark::DoNotOptimize(pit.freq());
            pit.tick();
        }
    }
}

BENCHMARK(BM_phase_increment_tracker);

TEST(minimum_performance, BM_phase_increment_tracker) {
    auto run = InterceptReporter::run_map.at("BM_phase_increment_tracker");
    EXPECT_LE(run, 0.004);  // 2500x faster than playback time
}

TEST(ideal_performance, BM_phase_increment_tracker) {
    auto run = InterceptReporter::run_map.at("BM_phase_increment_tracker");
    EXPECT_LE(run, 0.003);  // 3333x faster than playback time
}

static void BM_phase_increment_tracker_only_tick(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        auto pit = clover::dsp::phase_increment_tracker::for_freq(clover_bm::fs_48k, 100);
        state.ResumeTiming();

        for (int i = 0; i < clover_bm::samples_10s_48k; ++i) {
            benchmark::DoNotOptimize(pit.phase());
            pit.tick();
        }
    }
}

BENCHMARK(BM_phase_increment_tracker_only_tick);

TEST(minimum_performance, BM_phase_increment_tracker_only_tick) {
    auto run = InterceptReporter::run_map.at("BM_phase_increment_tracker_only_tick");
    EXPECT_LE(run, 0.0015);  // 6666x faster than playback time
}

TEST(ideal_performance, BM_phase_increment_tracker_only_tick) {
    auto run = InterceptReporter::run_map.at("BM_phase_increment_tracker_only_tick");
    EXPECT_LE(run, 0.001);  // 10000x faster than playback time
}
