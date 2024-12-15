// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/wave.hpp"

#include "clover_benchmark/util.hpp"

static void BM_wave_sine(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_sine(static_cast<clover_float>(element) * increment));
    }
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
static void BM_wave_saw(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_saw(static_cast<clover_float>(element) * increment));
    }
}

static void BM_wave_tri(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_tri(static_cast<clover_float>(element) * increment));
    }
}

static void BM_wave_noise(benchmark::State& state) {
    auto range                       = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr clover_float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range)
            benchmark::DoNotOptimize(clover::dsp::wave_noise(static_cast<clover_float>(element) * increment));
    }
}

bm_assert(
        BM_wave_sine,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 5000.   // target
);

bm_assert(
        BM_wave_square,
        clover_bm::duration / 25000.,  // min
        clover_bm::duration / 28000.   // target
);

bm_assert(
        BM_wave_saw,
        clover_bm::duration / 25000.,  // min
        clover_bm::duration / 28000.   // target
);

bm_assert(
        BM_wave_tri,
        clover_bm::duration / 10000.,  // min
        clover_bm::duration / 16000.   // target
);

bm_assert(
        BM_wave_noise,
        clover_bm::duration / 10000.,  // min
        clover_bm::duration / 14000.   // target
);
