// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/interpolate_sinc.hpp"

#include "clover/circular_buffer.hpp"
#include "clover_benchmark/util.hpp"

static void BM_interpolate_hann_window_64(benchmark::State& state) {
    std::vector<clover_float> window;
    window.resize(64, 0);

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            clover::dsp::hann_window_with_corner_control(window, 0.2);
        }
    }
}

static void BM_interpolate_sinc_64(benchmark::State& state) {
    std::vector<clover_float> sinc;
    sinc.resize(64, 0);

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            clover::dsp::sinc_function(sinc, 0.5);
        }
    }
}

static void BM_interpolate_hadamard_product_64(benchmark::State& state) {
    std::vector<clover_float> a;
    std::vector<clover_float> b;
    a.resize(64, 1.1);
    b.resize(64, 1.1);

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            clover::dsp::hadamard_product(a, b);
            benchmark::DoNotOptimize(1 + 1);
        }
    }
}

static void BM_interpolate_steady_64(benchmark::State& state) {
    std::vector<clover_float> window;
    window.resize(64, 0);
    std::vector<clover_float> sinc;
    sinc.resize(64, 0);
    std::vector<clover_float> signal;
    signal.resize(64, 0);
    clover::dsp::circular_buffer buffer{signal};

    clover::dsp::hann_window_with_corner_control(window, 0.2);
    clover::dsp::sinc_function(sinc, 0.5);

    for (auto i : std::views::iota(0, 64)) {
        sinc[i]   = sinc[i] * window[i];
        signal[i] = 0.2f;
    }

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            clover::dsp::interpolate_sinc(buffer, sinc);
        }
    }
}

static void BM_interpolate_busy_64(benchmark::State& state) {
    std::vector<clover_float> window;
    window.resize(64, 0);
    std::vector<clover_float> sinc;
    sinc.resize(64, 0);
    std::vector<clover_float> signal;
    signal.resize(64, 0.9);
    clover::dsp::circular_buffer buffer{signal};

    clover::dsp::hann_window_with_corner_control(window, 0.2);

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            auto pct = static_cast<clover_float>(i) / clover_bm::samples_10s_48k;

            clover::dsp::sinc_function(sinc, pct);
            clover::dsp::hadamard_product(sinc, window);

            clover::dsp::interpolate_sinc(buffer, sinc);
            buffer.tick(0.2f);
        }
    }
}

bm_assert(
        BM_interpolate_hann_window_64,
        clover_bm::duration / 200.,  // min
        clover_bm::duration / 200.   // target
);

bm_assert(
        BM_interpolate_sinc_64,
        clover_bm::duration / 90.,  // min
        clover_bm::duration / 90.   // target
);

bm_assert(
        BM_interpolate_hadamard_product_64,
        clover_bm::duration / 4700.,  // min
        clover_bm::duration / 4700.   // target
);

bm_assert(
        BM_interpolate_steady_64,
        clover_bm::duration / 200.,  // min
        clover_bm::duration / 200.   // target
);

bm_assert(
        BM_interpolate_busy_64,
        clover_bm::duration / 40.,  // min
        clover_bm::duration / 40.   // target
);