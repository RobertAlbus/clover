// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/circular_buffer.hpp"
#include "clover/dsp/fractional_delay.hpp"

#include "clover_benchmark/util.hpp"

void BM_fdl_lagrange_steady(benchmark::State& state) {
    std::vector<clover_float> signal;
    signal.resize(192000, 0);
    clover::dsp::circular_buffer buffer{signal};

    auto size = static_cast<clover_float>(signal.size());

    clover::dsp::fdl_lagrange fdl{buffer};

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            buffer.tick(static_cast<clover_float>(i));
            fdl.at(480.996f);
        }
    }
}

void BM_fdl_lagrange_busy(benchmark::State& state) {
    std::vector<clover_float> signal;
    signal.resize(192000, 0);
    clover::dsp::circular_buffer buffer{signal};

    auto size = static_cast<clover_float>(signal.size());

    clover_float max_index = size - 4;
    clover::dsp::fdl_lagrange fdl{buffer};

    clover_float index = 2;
    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            if (index >= max_index)
                index = 2;
            buffer.tick(static_cast<clover_float>(i));
            fdl.at(index);
            index += 0.13;
        }
    }
}

/*


*/

bm_assert(
        BM_fdl_lagrange_steady,
        clover_bm::duration / 1000.,  // min
        clover_bm::duration / 1000.   // target
);

bm_assert(
        BM_fdl_lagrange_busy,
        clover_bm::duration / 1000.,  // min
        clover_bm::duration / 1000.   // target
);
