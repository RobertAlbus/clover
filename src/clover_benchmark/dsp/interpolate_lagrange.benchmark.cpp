// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/interpolate_lagrange.hpp"

#include "clover_benchmark/util.hpp"

void BM_interpolate_lagrange(benchmark::State& state) {
    float p0 = -0.0809f;
    float p1 = -0.12345f;
    float p2 = 0.9834f;
    float p3 = -0.1f;

    float s = 0.2345f;

    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto i : range) {
            benchmark::DoNotOptimize(clover::dsp::interpolate_lagrange(p0, p1, p2, p3, s));
        }
    }
}

bm_assert(
        BM_interpolate_lagrange,
        clover_bm::duration / 9000.,  // min
        clover_bm::duration / 9000.   // target
);
