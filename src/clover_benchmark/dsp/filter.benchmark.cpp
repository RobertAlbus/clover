// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"

#include "clover/dsp/filter.hpp"

#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover_benchmark/util.hpp"

using namespace clover::dsp;

static void BM_filter_steady(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    filter filt{lpf(clover_bm::fs_48k, 9000, 0.8)};

    std::vector<float> signal;
    signal.reserve(clover_bm::samples_10s_48k);
    oscillator osc(clover_bm::fs_48k);
    osc.waveform = wave_square;
    osc.freq(500);

    for (const auto& i : range) {
        signal.emplace_back(osc.tick() * 0.7);
    }

    for (const auto& i : state) {
        for (auto x : signal) {
            benchmark::DoNotOptimize(filt.tick(x));
        }
    }
}

static void BM_filter_busy(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    filter filt{lpf(clover_bm::fs_48k, 9000, 0.8)};

    std::vector<float> signal;
    signal.reserve(clover_bm::samples_10s_48k);
    oscillator osc(clover_bm::fs_48k);
    osc.waveform = wave_square;
    osc.freq(500);

    for (const auto& i : range) {
        signal.emplace_back(osc.tick() * 0.7);
    }

    for (const auto& i : state) {
        for (auto x : signal) {
            filt.m_coeffs = lpf(clover_bm::fs_48k, 200 + (120 * x), 1);
            benchmark::DoNotOptimize(filt.tick(x));
        }
    }
}

static void BM_filter_2_steady(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    filter_2 filt{lpf(clover_bm::fs_48k, 9000, 0.8)};

    std::vector<float> signal;
    signal.reserve(clover_bm::samples_10s_48k);
    oscillator osc(clover_bm::fs_48k);
    osc.waveform = wave_square;
    osc.freq(500);

    for (const auto& i : range) {
        signal.emplace_back(osc.tick() * 0.7);
    }

    for (const auto& i : state) {
        for (auto x : signal) {
            benchmark::DoNotOptimize(filt.tick(x, x));
        }
    }
}

static void BM_filter_2_busy(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    filter_2 filt{lpf(clover_bm::fs_48k, 9000, 0.8)};

    std::vector<float> signal;
    signal.reserve(clover_bm::samples_10s_48k);
    oscillator osc(clover_bm::fs_48k);
    osc.waveform = wave_square;
    osc.freq(500);

    for (const auto& i : range) {
        signal.emplace_back(osc.tick() * 0.7);
    }

    for (const auto& i : state) {
        for (auto x : signal) {
            filt.m_coeffs = lpf(clover_bm::fs_48k, 200 + (120 * x), 1);
            benchmark::DoNotOptimize(filt.tick(x, x));
        }
    }
}

static void BM_filter_eq_steady(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    filter filt{eq(clover_bm::fs_48k, 9000, 0.8, -3)};

    std::vector<float> signal;
    signal.reserve(clover_bm::samples_10s_48k);
    oscillator osc(clover_bm::fs_48k);
    osc.waveform = wave_square;
    osc.freq(500);

    for (const auto& i : range) {
        signal.emplace_back(osc.tick() * 0.7);
    }

    for (const auto& i : state) {
        for (auto x : signal) {
            benchmark::DoNotOptimize(filt.tick(x));
        }
    }
}

static void BM_filter_eq_busy(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    filter filt{eq(clover_bm::fs_48k, 9000, 0.8, -3)};

    std::vector<float> signal;
    signal.reserve(clover_bm::samples_10s_48k);
    oscillator osc(clover_bm::fs_48k);
    osc.waveform = wave_square;
    osc.freq(500);

    for (const auto& i : range) {
        signal.emplace_back(osc.tick() * 0.7);
    }

    for (const auto& i : state) {
        for (auto x : signal) {
            filt.m_coeffs = eq(clover_bm::fs_48k, 200 + (120 * x), 1, -3);
            benchmark::DoNotOptimize(filt.tick(x));
        }
    }
}

static void BM_iir_coeffs_apf(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(apf(clover_bm::fs_48k, 9000, 0.8));
        }
    }
}

static void BM_iir_coeffs_lpf(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(lpf(clover_bm::fs_48k, 9000, 0.8));
        }
    }
}

static void BM_iir_coeffs_hpf(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(hpf(clover_bm::fs_48k, 9000, 0.8));
        }
    }
}

static void BM_iir_coeffs_bpf(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(bpf(clover_bm::fs_48k, 9000, 0.8));
        }
    }
}

static void BM_iir_coeffs_notch(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(notch(clover_bm::fs_48k, 9000, 0.8));
        }
    }
}

static void BM_iir_coeffs_ls(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(ls(clover_bm::fs_48k, 9000, 0.8, 3));
        }
    }
}

static void BM_iir_coeffs_hs(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(hs(clover_bm::fs_48k, 9000, 0.8, 3));
        }
    }
}

static void BM_iir_coeffs_eq(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (const auto& i : state) {
        for (auto x : range) {
            benchmark::DoNotOptimize(eq(clover_bm::fs_48k, 9000, 0.8, 3));
        }
    }
}

bm_assert(
        BM_filter_steady,
        clover_bm::duration / 7000.,  // min
        clover_bm::duration / 7000.   // target
);

bm_assert(
        BM_filter_busy,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3000.   // target
);

bm_assert(
        BM_filter_2_steady,
        clover_bm::duration / 3500.,  // min
        clover_bm::duration / 3500.   // target
);

bm_assert(
        BM_filter_2_busy,
        clover_bm::duration / 1500.,  // min
        clover_bm::duration / 1500.   // target
);

bm_assert(
        BM_filter_eq_steady,
        clover_bm::duration / 7000.,  // min
        clover_bm::duration / 7000.   // target
);

bm_assert(
        BM_filter_eq_busy,
        clover_bm::duration / 1700.,  // min
        clover_bm::duration / 1700.   // target
);

bm_assert(
        BM_iir_coeffs_apf,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3000.   // target
);

bm_assert(
        BM_iir_coeffs_lpf,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3000.   // target
);

bm_assert(
        BM_iir_coeffs_hpf,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3000.   // target
);

bm_assert(
        BM_iir_coeffs_bpf,
        clover_bm::duration / 3000.,  // min
        clover_bm::duration / 3000.   // target
);

bm_assert(
        BM_iir_coeffs_ls,
        clover_bm::duration / 1500.,  // min
        clover_bm::duration / 1500.   // target
);

bm_assert(
        BM_iir_coeffs_hs,
        clover_bm::duration / 1600.,  // min
        clover_bm::duration / 1600.   // target
);

bm_assert(
        BM_iir_coeffs_eq,
        clover_bm::duration / 1800.,  // min
        clover_bm::duration / 1800.   // target
);
