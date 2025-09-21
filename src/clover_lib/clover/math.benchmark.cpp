// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "benchmark/benchmark.h"
#include <sys/types.h>

#include "clover/math.hpp"
#include "clover/num.hpp"

#include "clover/clover_benchmark/util.hpp"

static void BM_db_curve(benchmark::State& state) {
    auto range                = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range) {
            auto x = static_cast<float>(element) * increment;
            benchmark::DoNotOptimize(clover::db_curve(x, 1));
        }
    }
}

static void BM_db_curve_inverse(benchmark::State& state) {
    auto range                = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;
    for (auto _ : state) {
        for (const auto& element : range) {
            auto x = static_cast<float>(element) * increment;
            benchmark::DoNotOptimize(clover::db_curve(x, 1));
        }
    }
}

static void BM_db_to_linear(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    float db = -80.f;

    for (auto _ : state) {
        for (const auto& _ : range) {
            benchmark::DoNotOptimize(clover::db_to_linear(db += 1.f));
            if (db >= 12.f)
                db -= 92.f;
        }
    }
}

static void BM_linear_to_db(benchmark::State& state) {
    auto range                = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k;

    for (auto _ : state) {
        for (const auto& x : range) {
            benchmark::DoNotOptimize(clover::db_to_linear(static_cast<float>(x) * increment));
        }
    }
}

static void BM_frequency_to_midi(benchmark::State& state) {
    auto range                = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k / 2;

    for (auto _ : state) {
        for (const auto& x : range) {
            benchmark::DoNotOptimize(clover::frequency_to_midi(static_cast<float>(x) * increment));
        }
    }
}

static void BM_midi_to_frequency(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));

    for (auto _ : state) {
        float note = 0;
        for (const auto& x : range) {
            benchmark::DoNotOptimize(clover::midi_to_frequency(note));
            note += 0.25;
            if (note >= 127.f)
                note -= 127.f;
        }
    }
}

static void bm_sign(benchmark::State& state) {
    int magnitude             = static_cast<int>(clover_bm::samples_10s_48k / 2.f);
    auto range                = std::views::iota(-magnitude, magnitude);
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k / 2.f;

    for (auto _ : state) {
        for (const auto& x : range) {
            benchmark::DoNotOptimize(clover::sign(static_cast<float>(x) * increment));
        }
    }
}

static void BM_tension(benchmark::State& state) {
    int magnitude             = static_cast<int>(clover_bm::samples_10s_48k / 2.f);
    auto range                = std::views::iota(-magnitude, magnitude);
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k / 2.f;

    for (auto _ : state) {
        for (const auto& x : range) {
            float arg = static_cast<float>(x) * increment;
            benchmark::DoNotOptimize(clover::tension(arg, arg));
        }
    }
}

static void BM_tension_pos_a(benchmark::State& state) {
    int magnitude             = static_cast<int>(clover_bm::samples_10s_48k / 2.f);
    auto range                = std::views::iota(-magnitude, magnitude);
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k / 2.f;

    for (auto _ : state) {
        for (const auto& x : range) {
            float arg = static_cast<float>(x) * increment;
            benchmark::DoNotOptimize(clover::tension_pos_a(arg, 0.2345));
        }
    }
}

static void BM_tension_neg_a(benchmark::State& state) {
    int magnitude             = static_cast<int>(clover_bm::samples_10s_48k / 2.f);
    auto range                = std::views::iota(-magnitude, magnitude);
    constexpr float increment = clover::num::pi_x2 / clover_bm::samples_10s_48k / 2.f;

    for (auto _ : state) {
        for (const auto& x : range) {
            float arg = static_cast<float>(x) * increment;
            benchmark::DoNotOptimize(clover::tension_neg_a(arg, 0.2345));
        }
    }
}

static void BM_frequency_by_octave_difference(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (const auto& element : range) {
            benchmark::DoNotOptimize(clover::frequency_by_octave_difference(456.98f, (7.f / 12.f)));
        }
    }
}

static void BM_frequency_by_semitone_difference(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (const auto& element : range) {
            benchmark::DoNotOptimize(clover::frequency_by_semitone_difference(456.98f, (7.f / 12.f)));
        }
    }
}

static void BM_octave_difference_by_frequency(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto element : range) {
            benchmark::DoNotOptimize(clover::octave_difference_by_frequency(300.99f, 9872.12f));
        }
    }
}

static void BM_semitone_difference_by_frequency(benchmark::State& state) {
    auto range = std::views::iota(0, static_cast<int>(clover_bm::samples_10s_48k));
    for (auto _ : state) {
        for (auto element : range) {
            benchmark::DoNotOptimize(clover::semitone_difference_by_frequency(300.99f, 9872.12f));
        }
    }
}

bm_assert(
        BM_db_curve,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 4500.   // target
);

bm_assert(
        BM_db_curve_inverse,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 4500.   // target
);

bm_assert(
        BM_db_to_linear,
        clover_bm::duration / 3200.,  // min
        clover_bm::duration / 3600.   // target
);

bm_assert(
        BM_linear_to_db,
        clover_bm::duration / 3600.,  // min
        clover_bm::duration / 4000.   // target
);

bm_assert(
        BM_frequency_by_octave_difference,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 4500.   // target
);

bm_assert(
        BM_frequency_by_semitone_difference,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 4500.   // target
);

bm_assert(
        BM_octave_difference_by_frequency,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 4500.   // target
);

bm_assert(
        BM_semitone_difference_by_frequency,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 4500.   // target
);

bm_assert(
        BM_frequency_to_midi,
        clover_bm::duration / 2800.,  // min
        clover_bm::duration / 3200.   // target
);

bm_assert(
        BM_midi_to_frequency,
        clover_bm::duration / 3200.,  // min
        clover_bm::duration / 3500.   // target
);

bm_assert(
        bm_sign,
        clover_bm::duration / 24000.,  // min
        clover_bm::duration / 27000.   // target
);

bm_assert(
        BM_tension,
        clover_bm::duration / 4000.,  // min
        clover_bm::duration / 5000.   // target
);

bm_assert(
        BM_tension_pos_a,
        clover_bm::duration / 16000.,  // min
        clover_bm::duration / 18000.   // target
);

bm_assert(
        BM_tension_neg_a,
        clover_bm::duration / 20000.,  // min
        clover_bm::duration / 22000.   // target
);
