// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "benchmark/benchmark.h"

#include "clover/dsp/phase_increment_tracker.hpp"

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

bm_assert(
        BM_phase_increment_tracker,
        clover_bm::duration / 2500.,  // min
        clover_bm::duration / 3333.   // target
);

bm_assert(
        BM_phase_increment_tracker_only_tick,
        clover_bm::duration / 6666.,  // min
        clover_bm::duration / 10000.  // target
);
