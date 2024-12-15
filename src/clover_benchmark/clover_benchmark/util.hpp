#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cstdint>

#include "gtest/gtest.h"  // used in bm_assert

#include "clover_benchmark/intercept_reporter.hpp"  // used in bm_assert

#define bm_assert(name, min_perf, target_perf)           \
    BENCHMARK(name);                                     \
    TEST(minimum_performance, name) {                    \
        auto run = InterceptReporter::run_map.at(#name); \
        EXPECT_LE(run, min_perf);                        \
    }                                                    \
    TEST(ideal_performance, name) {                      \
        auto run = InterceptReporter::run_map.at(#name); \
        EXPECT_LE(run, target_perf);                     \
    }

namespace clover_bm {

using bm_int = uint_fast16_t;

constexpr bm_int fs_48k          = 48000;
constexpr bm_int seconds_10      = 10;
constexpr bm_int samples_10s_48k = fs_48k * seconds_10;
constexpr bm_int duration        = seconds_10;

}  // namespace clover_bm
