// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/dsp/interpolate_sinc.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_interpolate_sinc, hann_window) {
    clover_float alpha = 0.2;
    std::vector<clover_float> window;
    window.resize(63, 0);

    hann_window_with_corner_control(window, alpha);

    EXPECT_FLOAT_EQ(window[0], alpha);
    EXPECT_FLOAT_EQ(window[62], alpha);
    EXPECT_FLOAT_EQ(window[31], 1);

    for (auto x : window) {
        EXPECT_GE(x, alpha);
        EXPECT_LE(x, 1);
    }

    window.resize(64, 0);
    hann_window_with_corner_control(window, alpha);

    EXPECT_FLOAT_EQ(window[0], alpha);
    EXPECT_FLOAT_EQ(window[63], alpha);
    EXPECT_FLOAT_EQ(window[31], window[32]);

    for (auto x : window) {
        EXPECT_GE(x, alpha);
        EXPECT_LE(x, window[31]);
    }
}

TEST(dsp_interpolate_sinc, hann_window_to_rectangle) {
    clover_float alpha = 1;
    std::vector<clover_float> window;
    window.resize(64, 0);
    hann_window_with_corner_control(window, alpha);

    for (auto x : window) {
        EXPECT_FLOAT_EQ(x, 1);
    }
}

TEST(dsp_interpolate_sinc, sinc_function) {
    std::vector<clover_float> sinc;

    sinc.resize(4, 0);
    sinc_function(sinc, 0);
    for (auto i : std::views::iota(0, 4)) {
        if (i == 1)
            EXPECT_FLOAT_EQ(sinc[i], 1);
        else
            EXPECT_NEAR(sinc[i], 0, 2.8e-08);
    }

    sinc_function(sinc, 1);
    for (auto i : std::views::iota(0, 4)) {
        if (i == 2)
            EXPECT_FLOAT_EQ(sinc[i], 1);
        else
            EXPECT_NEAR(sinc[i], 0, 2.8e-08);
    }

    sinc.resize(8, 0);
    sinc_function(sinc, 0);
    for (auto i : std::views::iota(0, 8)) {
        if (i == 3)
            EXPECT_FLOAT_EQ(sinc[i], 1);
        else
            EXPECT_NEAR(sinc[i], 0, 2.8e-08);
    }

    sinc_function(sinc, 1);
    for (auto i : std::views::iota(0, 8)) {
        if (i == 4)
            EXPECT_FLOAT_EQ(sinc[i], 1);
        else
            EXPECT_NEAR(sinc[i], 0, 2.8e-08);
    }
}

TEST(dsp_interpolate_sinc, hadamard_product) {
    std::vector<clover_float> a{1, 2, 3};
    std::vector<clover_float> b{4, 5, 6};

    hadamard_product(a, b);

    EXPECT_FLOAT_EQ(a[0], 4);
    EXPECT_FLOAT_EQ(a[1], 10);
    EXPECT_FLOAT_EQ(a[2], 18);
}