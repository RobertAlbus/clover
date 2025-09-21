// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"
#include <sys/types.h>

#include "clover/float.hpp"

#include "clover/clover_benchmark/util.hpp"

static void BM_float_eq(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (const auto& element : range) {
            benchmark::DoNotOptimize(clover::float_eq(static_cast<float>(element), 1));
        }
    }
}

bm_assert(
        BM_float_eq,
        clover_bm::duration / 23000.,  // min
        clover_bm::duration / 26000.   // target
);
