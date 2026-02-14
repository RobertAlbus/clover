// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/wavetable.hpp"
#include "clover/dsp/wavetable_oscillator.hpp"

#include "clover/clover_benchmark/util.hpp"

static void BM_wavetable_oscillator(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    auto wt    = clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine);
    wt.freq(20.965f);

    for (const auto& _ : state) {
        float new_freq = wt.freq() * 3.86f;
        if (new_freq > clover_bm::nyquist_48k)
            new_freq -= clover_bm::nyquist_48k;
        wt.freq(new_freq);

        float freq_state = new_freq;
        for (const auto& i : range) {
            freq_state *= 3.9f;
            if (freq_state > clover_bm::nyquist_48k)
                freq_state -= clover_bm::nyquist_48k;
            wt.freq(freq_state);
            benchmark::DoNotOptimize(wt.tick());
        }
    }
}

static void BM_wavetable_oscillator_steady(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    auto wt    = clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine);
    wt.freq(546.965f);

    for (const auto& _ : state) {
        float new_freq = wt.freq() * 3.86f;
        if (new_freq > clover_bm::nyquist_48k)
            new_freq -= clover_bm::nyquist_48k;
        wt.freq(new_freq);

        for (const auto& i : range) {
            benchmark::DoNotOptimize(wt.tick());
        }
    }
}

static void BM_wavetable_oscillator_x8(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    clover::dsp::wavetable_oscillator oscs[] = {
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
    };

    float base_freq = 20.965f;
    for (size_t i = 0; i < 8; ++i) {
        oscs[i].freq(base_freq * static_cast<float>(i + 1));
    }

    for (const auto& _ : state) {
        float freq_state = base_freq;
        for (const auto& i : range) {
            freq_state *= 3.9f;
            if (freq_state > clover_bm::nyquist_48k)
                freq_state -= clover_bm::nyquist_48k;
            for (size_t lane = 0; lane < 8; ++lane) {
                oscs[lane].freq(freq_state * static_cast<float>(lane + 1));
                benchmark::DoNotOptimize(oscs[lane].tick());
            }
        }
    }
}

static void BM_wavetable_oscillator_x8_mixed(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    clover::dsp::wavetable_oscillator oscs[] = {
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::sine),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::square),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::saw),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::tri),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::noise),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::square),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::saw),
            clover::dsp::wavetable_oscillator(clover_bm::fs_48k, clover::dsp::wavetable::tri),
    };

    for (size_t i = 0; i < 8; ++i) {
        oscs[i].freq(546.965f * static_cast<float>(i + 1));
    }

    for (const auto& _ : state) {
        for (const auto& i : range) {
            for (auto& osc : oscs) {
                benchmark::DoNotOptimize(osc.tick());
            }
        }
    }
}

bm_assert(
        BM_wavetable_oscillator,
        clover_bm::duration / 2200.,  // min
        clover_bm::duration / 2600.   // target
);

bm_assert(
        BM_wavetable_oscillator_steady,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3500.   // target
);

bm_assert(
        BM_wavetable_oscillator_x8,
        clover_bm::duration / 2200.,  // min
        clover_bm::duration / 2600.   // target
);

bm_assert(
        BM_wavetable_oscillator_x8_mixed,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3500.   // target
);
