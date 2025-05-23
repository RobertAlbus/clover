// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/pan.hpp"

#include "clover_benchmark/util.hpp"

void BM_pan_steady(benchmark::State& state) {
    auto limit = static_cast<int>(clover_bm::samples_10s_48k) / 2;
    auto range = std::views::iota(-limit, limit);

    auto limit_recip = 1.f / static_cast<clover_float>(limit);
    clover::dsp::pan p;

    for (const auto& i : state) {
        for (const auto& i : range) {
            benchmark::DoNotOptimize(p.process(static_cast<clover_float>(i)));
        }
    }
}

void BM_pan_busy(benchmark::State& state) {
    auto limit = static_cast<int>(clover_bm::samples_10s_48k) / 2;
    auto range = std::views::iota(-limit, limit);

    auto limit_recip = 1.f / static_cast<clover_float>(limit);
    clover::dsp::pan p;

    for (const auto& i : state) {
        for (const auto& i : range) {
            p.set(limit_recip * static_cast<clover_float>(i));
            benchmark::DoNotOptimize(p.process(static_cast<clover_float>(i)));
        }
    }
}

bm_assert(
        BM_pan_steady,
        clover_bm::duration / 40000.,  // min
        clover_bm::duration / 40000.   // target
);

bm_assert(
        BM_pan_busy,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3000.   // target
);
