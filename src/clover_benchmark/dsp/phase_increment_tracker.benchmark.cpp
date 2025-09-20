// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/phase_increment_tracker.hpp"

#include "clover_benchmark/util.hpp"

static void BM_phase_increment_tracker(benchmark::State& state) {
    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        auto pit   = clover::dsp::phase_increment_tracker::for_freq(clover_bm::fs_48k, 100);

        for (const auto& i : range) {
            pit.phase(static_cast<float>(i % 6));
            pit.freq(100 + static_cast<float>(i % 6) * 100);
            benchmark::DoNotOptimize(pit.phase());
            benchmark::DoNotOptimize(pit.phase_offset());
            benchmark::DoNotOptimize(pit.freq());
            pit.tick();
        }
    }
}

static void BM_phase_increment_tracker_only_tick(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    auto pit   = clover::dsp::phase_increment_tracker::for_freq(clover_bm::fs_48k, 100);

    for (auto _ : state) {
        for (const auto& i : range) {
            benchmark::DoNotOptimize(pit.phase());
            pit.tick();
        }
    }
}

bm_assert(
        BM_phase_increment_tracker,
        clover_bm::duration / 3500.,  // min
        clover_bm::duration / 3500.   // target
);

bm_assert(
        BM_phase_increment_tracker_only_tick,
        clover_bm::duration / 11000.,  // min
        clover_bm::duration / 11000.   // target
);
