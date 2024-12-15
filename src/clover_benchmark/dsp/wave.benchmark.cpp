// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"
#include "gtest/gtest.h"

#include "clover/dsp/wave.hpp"

#include "clover_benchmark/intercept_reporter.hpp"
#include "clover_benchmark/util.hpp"

static void BM_wave_sine(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_sine(static_cast<clover_float>(element) * increment));
    }
}

BENCHMARK(BM_wave_sine);

TEST(minimum_performance, BM_wave_sine) {
    auto run = InterceptReporter::run_map.at("BM_wave_sine");
    EXPECT_LE(run, 0.0025);  // 4000x faster than playback time
}

TEST(ideal_performance, BM_wave_sine) {
    auto run = InterceptReporter::run_map.at("BM_wave_sine");
    EXPECT_LE(run, 0.0018);  // 5000x faster than playback time
}

static void BM_wave_square(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(
                    clover::dsp::wave_square(static_cast<clover_float>(element) * increment));
    }
}

BENCHMARK(BM_wave_square);

TEST(minimum_performance, BM_wave_square) {
    auto run = InterceptReporter::run_map.at("BM_wave_square");
    EXPECT_LE(run, 0.0004);  // 25000x faster than playback time
}

TEST(ideal_performance, BM_wave_square) {
    auto run = InterceptReporter::run_map.at("BM_wave_square");
    EXPECT_LE(run, 0.00035);  // 28000x faster than playback time
}

static void BM_wave_saw(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_saw(static_cast<clover_float>(element) * increment));
    }
}

BENCHMARK(BM_wave_saw);

TEST(minimum_performance, BM_wave_saw) {
    auto run = InterceptReporter::run_map.at("BM_wave_saw");
    EXPECT_LE(run, 0.0004);  // 25000x faster than playback time
}

TEST(ideal_performance, BM_wave_saw) {
    auto run = InterceptReporter::run_map.at("BM_wave_saw");
    EXPECT_LE(run, 0.00035);  // 28000x faster than playback time
}

static void BM_wave_tri(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_tri(static_cast<clover_float>(element) * increment));
    }
}

BENCHMARK(BM_wave_tri);

TEST(minimum_performance, BM_wave_tri) {
    auto run = InterceptReporter::run_map.at("BM_wave_tri");
    EXPECT_LE(run, 0.001);  // 10000x faster than playback time
}

TEST(ideal_performance, BM_wave_tri) {
    auto run = InterceptReporter::run_map.at("BM_wave_tri");
    EXPECT_LE(run, 0.0006);  // 16000x faster than playback time
}

static void BM_wave_noise(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_noise(static_cast<clover_float>(element) * increment));
    }
}

BENCHMARK(BM_wave_noise);

TEST(minimum_performance, BM_wave_noise) {
    auto run = InterceptReporter::run_map.at("BM_wave_noise");
    EXPECT_LE(run, 0.001);  // 10000x faster than playback time
}

TEST(ideal_performance, BM_wave_noise) {
    auto run = InterceptReporter::run_map.at("BM_wave_noise");
    EXPECT_LE(run, 0.0007);  // 14000x faster than playback time
}
