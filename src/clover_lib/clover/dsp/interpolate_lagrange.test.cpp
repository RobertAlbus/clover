// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/interpolate_lagrange.hpp"
#include <cmath>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/num.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_interpolate_lagrange, interpolate) {
    std::vector<float> sin_1khz;
    sin_1khz.resize(48000, 0);

    float t = 1000.0f / 48000.0f;
    for (auto i : std::views::iota(0, 48000))
        sin_1khz[i] = std::sin(2 * num::pi * t * static_cast<float>(i));

    float interpolation_amount = 0.44;
    for (auto i : std::views::iota(0, 48000 - 4)) {
        auto i_f           = static_cast<float>(i);
        float interpolated = interpolate_lagrange(
                sin_1khz[i + 0], sin_1khz[i + 1], sin_1khz[i + 2], sin_1khz[i + 3], interpolation_amount);
        float expected = std::sin(2 * num::pi * t * (1 + i_f + interpolation_amount));

        float tolerance = 5.5e-03;
        EXPECT_NEAR(interpolated, expected, tolerance);
    }
}
