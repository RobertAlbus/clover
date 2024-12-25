// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/env_automation_clip.hpp"

#include "clover_benchmark/util.hpp"

using namespace clover::dsp;

static void BM_env_automation_clip(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    env_automation_clip env{
            {0, 0},
            {1 * 48000, 100},
            {2 * 48000, 0},
            {3 * 48000, 900},
            {4 * 48000, 5},
            {5 * 48000, 2},
            {6 * 48000, 500},
            {7 * 48000, 6},
            {8 * 48000, 0},
            {9 * 48000, -500},
    };

    for (const auto& i : state) {
        for (const auto& i : range) {
            benchmark::DoNotOptimize(env.tick());
        }
    }
}

bm_assert(
        BM_env_automation_clip,
        clover_bm::duration / 7000.,  // min
        clover_bm::duration / 7000.   // target
);
