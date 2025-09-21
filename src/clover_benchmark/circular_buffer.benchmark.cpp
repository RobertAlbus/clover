// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/circular_buffer.hpp"

#include "clover_benchmark/util.hpp"

static void BM_circular_buffer_read_write(benchmark::State& state) {
    clover::dsp::circular_buffer buffer{96000};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));

    for (auto _ : state) {
        int ii = 0;
        for (auto i : range) {
            buffer.tick(float(i));
            benchmark::DoNotOptimize(buffer[ii]);
            if (++ii == 96000)
                ii -= 96000;
        }
    }
}

static void BM_circular_buffer_reads(benchmark::State& state) {
    clover::dsp::circular_buffer buffer{96000};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));
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
    clover::dsp::circular_buffer buffer{96000};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto i : range) {
            buffer.tick(float(i));
        }
    }
}

static void BM_circular_buffer_2_read_write(benchmark::State& state) {
    clover::dsp::circular_buffer_2 buffer{96000};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));

    for (auto _ : state) {
        int ii = 0;
        for (auto i : range) {
            buffer.tick({float(i), float(i)});
            benchmark::DoNotOptimize(buffer[ii]);
            if (++ii == 96000)
                ii -= 96000;
        }
    }
}

static void BM_circular_buffer_2_reads(benchmark::State& state) {
    clover::dsp::circular_buffer_2 buffer{96001};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));
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
    clover::dsp::circular_buffer_2 buffer{96001};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto i : range) {
            buffer.tick({float(i), float(i)});
        }
    }
}

bm_assert(
        BM_circular_buffer_read_write,
        clover_bm::duration / 7500.,  // min
        clover_bm::duration / 9000.   // target
);

bm_assert(
        BM_circular_buffer_reads,
        clover_bm::duration / 12000.,  // min
        clover_bm::duration / 14000.   // target
);

bm_assert(
        BM_circular_buffer_writes,
        clover_bm::duration / 22000.,  // min
        clover_bm::duration / 25000.   // target
);

bm_assert(
        BM_circular_buffer_2_read_write,
        clover_bm::duration / 7000.,  // min
        clover_bm::duration / 8000.   // target
);

bm_assert(
        BM_circular_buffer_2_reads,
        clover_bm::duration / 12500.,  // min
        clover_bm::duration / 14500.   // target
);

bm_assert(
        BM_circular_buffer_2_writes,
        clover_bm::duration / 19000.,  // min
        clover_bm::duration / 22000.   // target
);

static void BM_circular_buffer_t_double(benchmark::State& state) {
    clover::dsp::circular_buffer_t<double> buffer{96000};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto i : range) {
            buffer.tick(double(i) + 0.5);
        }
    }
}

static void BM_circular_buffer_t_int(benchmark::State& state) {
    clover::dsp::circular_buffer_t<int> buffer{96000};

    auto range = std::views::iota(0, int(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto i : range) {
            buffer.tick(i);
        }
    }
}

bm_assert(
        BM_circular_buffer_t_double,
        clover_bm::duration / 19000.,  // min
        clover_bm::duration / 22000.   // target
);

bm_assert(
        BM_circular_buffer_t_int,
        clover_bm::duration / 22000.,  // min
        clover_bm::duration / 25000.   // target
);