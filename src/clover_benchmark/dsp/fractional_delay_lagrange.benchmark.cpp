// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/fractional_delay.hpp"

#include "clover_benchmark/util.hpp"

void BM_fdl_lagrange_steady(benchmark::State& state) {
    clover::dsp::fdl_lagrange fdl{192000};

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            fdl.tick(static_cast<float>(i));
            fdl.at(480.996f);
        }
    }
}

void BM_fdl_lagrange_busy(benchmark::State& state) {
    size_t size    = 192000;
    auto max_index = float(size - 4);
    clover::dsp::fdl_lagrange fdl{192000};

    float index = 2;
    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            if (index >= max_index)
                index = 2;
            fdl.tick(static_cast<float>(i));
            fdl.at(index);
            index += 0.13;
        }
    }
}

void BM_fdl_lagrange_2_steady(benchmark::State& state) {
    clover::dsp::fdl_lagrange_2 fdl{284000};

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            fdl.tick(static_cast<float>(i), static_cast<float>(i));
            fdl.at(480.996f);
        }
    }
}

void BM_fdl_lagrange_2_busy(benchmark::State& state) {
    clover::dsp::fdl_lagrange_2 fdl{284000};
    float max_index = fdl.m_max_idx;

    float index = 2;
    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            if (index >= max_index)
                index = 2;
            fdl.tick(static_cast<float>(i), static_cast<float>(i));
            fdl.at(index);
            index += 0.13;
        }
    }
}

bm_assert(
        BM_fdl_lagrange_steady,
        clover_bm::duration / 800.,  // min
        clover_bm::duration / 900.   // target
);

bm_assert(
        BM_fdl_lagrange_busy,
        clover_bm::duration / 800.,  // min
        clover_bm::duration / 900.   // target
);

bm_assert(
        BM_fdl_lagrange_2_steady,
        clover_bm::duration / 800.,  // min
        clover_bm::duration / 900.   // target
);

bm_assert(
        BM_fdl_lagrange_2_busy,
        clover_bm::duration / 800.,  // min
        clover_bm::duration / 900.   // target
);