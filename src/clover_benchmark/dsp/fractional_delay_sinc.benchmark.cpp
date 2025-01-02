// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/circular_buffer.hpp"
#include "clover/dsp/fractional_delay.hpp"

#include "clover_benchmark/util.hpp"

void interpolator_sinc_benchmark(benchmark::State& state, size_t kernel_size, const bool act_busy) {
    std::vector<clover_float> signal =
            std::views::iota(-96000, 96000 - 1) |
            std::views::transform([](int x) { return static_cast<clover_float>(x); }) |
            std::ranges::to<std::vector>();
    clover::dsp::circular_buffer buffer{signal};

    auto size = static_cast<clover_float>(signal.size());

    clover::dsp::fdl_sinc fdl{buffer, kernel_size};

    for (auto _ : state) {
        auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
        for (auto i : range) {
            buffer.tick(static_cast<clover_float>(i));
            benchmark::DoNotOptimize(fdl.calculate());
            if (act_busy)
                fdl.delay(static_cast<clover_float>(i) / size);
        }
    }
}

void BM_fdl_sinc_steady_4(benchmark::State& state) {
    interpolator_sinc_benchmark(state, 4, false);
}

void BM_fdl_sinc_busy_4(benchmark::State& state) {
    interpolator_sinc_benchmark(state, 4, true);
}

void BM_fdl_sinc_steady_8(benchmark::State& state) {
    interpolator_sinc_benchmark(state, 8, false);
}

void BM_fdl_sinc_busy_8(benchmark::State& state) {
    interpolator_sinc_benchmark(state, 8, true);
}

void BM_fdl_sinc_steady_64(benchmark::State& state) {
    interpolator_sinc_benchmark(state, 64, false);
}

void BM_fdl_sinc_busy_64(benchmark::State& state) {
    interpolator_sinc_benchmark(state, 64, true);
}

bm_assert(
        BM_fdl_sinc_steady_4,
        clover_bm::duration / 1500.,  // min
        clover_bm::duration / 1500.   // target
);

bm_assert(
        BM_fdl_sinc_busy_4,
        clover_bm::duration / 750.,  // min
        clover_bm::duration / 750.   // target
);

bm_assert(
        BM_fdl_sinc_steady_8,
        clover_bm::duration / 1000.,  // min
        clover_bm::duration / 1000.   // target
);

bm_assert(
        BM_fdl_sinc_busy_8,
        clover_bm::duration / 400.,  // min
        clover_bm::duration / 400.   // target
);

bm_assert(
        BM_fdl_sinc_steady_64,
        clover_bm::duration / 170.,  // min
        clover_bm::duration / 170.   // target
);

bm_assert(
        BM_fdl_sinc_busy_64,
        clover_bm::duration / 50.,  // min
        clover_bm::duration / 50.   // target
);
