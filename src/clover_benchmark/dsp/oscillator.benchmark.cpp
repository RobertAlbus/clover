// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/oscillator.hpp"

#include "clover_benchmark/util.hpp"

static void BM_oscillator(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    auto osc   = clover::dsp::oscillator(clover_bm::fs_48k);
    osc.freq(20.965f);

    for (const auto& i : state) {
        float new_freq = osc.freq() * 3.86f;
        if (new_freq > clover_bm::nyquist_48k)
            new_freq -= clover_bm::nyquist_48k;
        osc.freq(new_freq);

        // using a state variable to simulate an external modulator
        float freq_state = new_freq;
        for (const auto& i : range) {
            freq_state *= 3.9f;
            if (freq_state > clover_bm::nyquist_48k)
                freq_state -= clover_bm::nyquist_48k;
            osc.freq(freq_state);
            benchmark::DoNotOptimize(osc.tick());
        }
    }
}

static void BM_oscillator_steady(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    auto osc   = clover::dsp::oscillator(clover_bm::fs_48k);
    osc.freq(546.965f);

    for (const auto& i : state) {
        float new_freq = osc.freq() * 3.86f;
        if (new_freq > clover_bm::nyquist_48k)
            new_freq -= clover_bm::nyquist_48k;
        osc.freq(new_freq);

        for (const auto& i : range) {
            benchmark::DoNotOptimize(osc.tick());
        }
    }
}

bm_assert(
        BM_oscillator,
        clover_bm::duration / 2200.,  // min
        clover_bm::duration / 2600.   // target
);

bm_assert(
        BM_oscillator_steady,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3500.   // target
);
