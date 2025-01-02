// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/circular_buffer.hpp"
#include "clover/float.hpp"
#include <ranges>

#include "benchmark/benchmark.h"
#include <sys/types.h>

#include "clover_benchmark/util.hpp"

static void BM_circular_buffer_read_write(benchmark::State& state) {
    std::vector<clover_float> underlying;
    benchmark::DoNotOptimize(underlying);
    underlying.resize(96000, 0);

    clover::dsp::circular_buffer buffer{underlying};

    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (auto _ : state) {
        int ii = 0;
        for (auto i : range) {
            buffer.tick(static_cast<clover_float>(i));
            benchmark::DoNotOptimize(buffer[ii]);
            if (++ii == 96000)
                ii -= 96000;
        }
    }
}

static void BM_circular_buffer_reads(benchmark::State& state) {
    std::vector<clover_float> underlying;
    benchmark::DoNotOptimize(underlying);
    underlying.resize(96000, 0);

    clover::dsp::circular_buffer buffer{underlying};

    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        int ii = 0;
        for (auto i : range) {
            benchmark::DoNotOptimize(buffer[ii]);
            if (++ii == 96000)
                ii -= 96000;
        }
    }
}

static void BM_circular_buffer_writes(benchmark::State& state) {
    std::vector<clover_float> underlying;
    benchmark::DoNotOptimize(underlying);

    underlying.resize(96000, 0);

    clover::dsp::circular_buffer buffer{underlying};

    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto i : range) {
            buffer.tick(static_cast<clover_float>(i));
        }
    }
}

static void BM_circular_buffer_2_read_write(benchmark::State& state) {
    std::vector<clover_float> underlying;
    benchmark::DoNotOptimize(underlying);
    underlying.resize(1920000, 0);

    clover::dsp::circular_buffer_2 buffer{underlying};

    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (auto _ : state) {
        int ii = 0;
        for (auto i : range) {
            buffer.tick(static_cast<clover_float>(i), static_cast<clover_float>(i));
            benchmark::DoNotOptimize(buffer[ii]);
            if (++ii == 96000)
                ii -= 96000;
        }
    }
}

static void BM_circular_buffer_2_reads(benchmark::State& state) {
    std::vector<clover_float> underlying;
    benchmark::DoNotOptimize(underlying);
    underlying.resize(1920000, 0);

    clover::dsp::circular_buffer_2 buffer{underlying};

    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        int ii = 0;
        for (auto i : range) {
            benchmark::DoNotOptimize(buffer[ii]);
            if (++ii == 96000)
                ii -= 96000;
        }
    }
}

static void BM_circular_buffer_2_writes(benchmark::State& state) {
    std::vector<clover_float> underlying;
    benchmark::DoNotOptimize(underlying);
    underlying.resize(1920000, 0);

    clover::dsp::circular_buffer_2 buffer{underlying};

    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto i : range) {
            buffer.tick(static_cast<clover_float>(i), static_cast<clover_float>(i));
        }
    }
}

bm_assert(
        BM_circular_buffer_read_write,
        clover_bm::duration / 10000.,  // min
        clover_bm::duration / 10000.   // target
);

bm_assert(
        BM_circular_buffer_reads,
        clover_bm::duration / 16000.,  // min
        clover_bm::duration / 16000.   // target
);

bm_assert(
        BM_circular_buffer_writes,
        clover_bm::duration / 28000.,  // min
        clover_bm::duration / 28000.   // target
);

bm_assert(
        BM_circular_buffer_2_read_write,
        clover_bm::duration / 9000.,  // min
        clover_bm::duration / 9000.   // target
);

bm_assert(
        BM_circular_buffer_2_reads,
        clover_bm::duration / 16000.,  // min
        clover_bm::duration / 16000.   // target
);

bm_assert(
        BM_circular_buffer_2_writes,
        clover_bm::duration / 24000.,  // min
        clover_bm::duration / 24000.   // target
);