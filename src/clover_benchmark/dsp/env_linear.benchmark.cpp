// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/env_linear.hpp"

#include "clover_benchmark/util.hpp"

using namespace clover::dsp;

static void BM_env_linear_busy(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    env_linear env;

    uint8_t choice   = 0;
    uint16_t counter = 0;

    for (const auto& i : state) {
        for (const auto& i : range) {
            counter++;
            if (counter == 480) {
                counter = 0;
                switch (choice) {
                    case 0:
                        env.set(30, 999, 4800);
                        ++choice;
                    case 1:
                        env.set(888, 4800);
                        ++choice;
                    case 2:
                        env.set(777);
                        choice = 0;
                }
            }
            benchmark::DoNotOptimize(env.tick());
        }
    }
}

static void BM_env_linear_steady(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    env_linear env;
    env.set(30, 999, 4800);

    uint16_t counter = 0;
    for (const auto& i : state) {
        for (const auto& i : range) {
            counter++;
            if (!counter)
                env.set(30, 999, 4800);

            benchmark::DoNotOptimize(env.tick());
        }
    }
}

bm_assert(
        BM_env_linear_busy,
        clover_bm::duration / 9000.,  // min
        clover_bm::duration / 9000.   // target
);

bm_assert(
        BM_env_linear_steady,
        clover_bm::duration / 30000.,  // min
        clover_bm::duration / 30000.   // target
);
