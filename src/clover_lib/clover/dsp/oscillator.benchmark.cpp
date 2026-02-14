// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"

#include "clover/clover_benchmark/util.hpp"

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

static void BM_oscillator_x8(benchmark::State& state) {
    using namespace clover::dsp;
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    oscillator oscs[] = {
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
    };

    oscs[0].waveform = wave_sine;
    oscs[1].waveform = wave_square;
    oscs[2].waveform = wave_saw;
    oscs[3].waveform = wave_tri;
    oscs[4].waveform = wave_noise;
    oscs[5].waveform = wave_square;
    oscs[6].waveform = wave_saw;
    oscs[7].waveform = wave_tri;

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

static void BM_oscillator_x8_steady(benchmark::State& state) {
    using namespace clover::dsp;
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    oscillator oscs[] = {
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
            oscillator(clover_bm::fs_48k),
    };

    oscs[0].waveform = wave_sine;
    oscs[1].waveform = wave_square;
    oscs[2].waveform = wave_saw;
    oscs[3].waveform = wave_tri;
    oscs[4].waveform = wave_noise;
    oscs[5].waveform = wave_square;
    oscs[6].waveform = wave_saw;
    oscs[7].waveform = wave_tri;

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
        BM_oscillator,
        clover_bm::duration / 2200.,  // min
        clover_bm::duration / 2600.   // target
);

bm_assert(
        BM_oscillator_steady,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3500.   // target
);

bm_assert(
        BM_oscillator_x8,
        clover_bm::duration / 2200.,  // min
        clover_bm::duration / 2600.   // target
);

bm_assert(
        BM_oscillator_x8_steady,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3500.   // target
);
