// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/env_adsr.hpp"

#include "clover_benchmark/util.hpp"

using namespace clover::dsp;

static void BM_env_adsr_busy(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    env_adsr env;
    env.set(480, 4800, 0.7, 20000);

    int counter = 0;

    for (const auto& i : state) {
        for (const auto& i : range) {
            if (counter == 0) {
                env.key_on();
            } else if (counter == 6000) {
                env.key_off();
            } else if (counter == 26000) {
                counter = -1;
            }
            ++counter;
            benchmark::DoNotOptimize(env.tick());
        }
    }
}

static void BM_env_adsr_steady(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    env_adsr env;
    env.set(4800, 4800, 0.7, 4800);

    int counter = 0;

    for (const auto& i : state) {
        for (const auto& i : range) {
            if (counter == 0) {
                env.key_on();
            } else if (counter == 2000000) {
                env.key_off();
            } else if (counter == 10000000) {
                counter = -1;
            }
            ++counter;
            benchmark::DoNotOptimize(env.tick());
        }
    }
}

bm_assert(
        BM_env_adsr_busy,
        clover_bm::duration / 6000.,  // min
        clover_bm::duration / 8000.   // target
);

bm_assert(
        BM_env_adsr_steady,
        clover_bm::duration / 10000.,  // min
        clover_bm::duration / 12000.   // target
);
