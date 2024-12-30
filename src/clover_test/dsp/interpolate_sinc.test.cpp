// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/dsp/interpolate_sinc.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_interpolate_sinc, hann_window) {
    clover_float alpha = 0.2;
    std::vector<clover_float> window;
    window.resize(63, 0);

    hann_window_with_corner_control(window, alpha);

    for (auto x : window)
        std::cout << x << std::endl;

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

TEST(dsp_interpolate_sinc, sinc_function) {
    std::vector<clover_float> sinc;
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

TEST(dsp_interpolate_sinc, interpolate_16) {
    std::vector<clover_float> window;
    std::vector<clover_float> sinc_kernel;

    window.resize(16, 0);
    sinc_kernel.resize(16, 0);

    hann_window_with_corner_control(window, 0.2);
    sinc_function(sinc_kernel, 0);
    for (auto i : std::views::iota(0, 16))
        sinc_kernel[i] = sinc_kernel[i] * window[i];

    oscillator osc(48000);
    osc.freq(9000);
    osc.waveform = wave_tri;
    std::vector<clover_float> signal;
    signal.resize(16, 0);

    circular_buffer buffer{signal};
    for (auto i : std::views::iota(0, 16))
        buffer.tick(osc.tick());

    clover_float interpolated = interpolate_sinc(buffer, sinc_kernel);
    EXPECT_NEAR(interpolated, buffer[7], .000073);

    sinc_function(sinc_kernel, 1);
    for (auto i : std::views::iota(0, 16))
        sinc_kernel[i] = sinc_kernel[i] * window[i];

    interpolated = interpolate_sinc(buffer, sinc_kernel);
    EXPECT_NEAR(interpolated, buffer[8], 0.0066);
}

TEST(dsp_interpolate_sinc, hadamard_product) {
    std::vector<clover_float> a{1, 2, 3};
    std::vector<clover_float> b{4, 5, 6};

    hadamard_product(a, b);

    EXPECT_FLOAT_EQ(a[0], 4);
    EXPECT_FLOAT_EQ(a[1], 10);
    EXPECT_FLOAT_EQ(a[2], 18);
}

TEST(dsp_interpolate_sinc, interpolate_64) {
    std::vector<clover_float> window;
    std::vector<clover_float> sinc_kernel;

    window.resize(64, 0);
    sinc_kernel.resize(64, 0);

    hann_window_with_corner_control(window, 0.2);
    sinc_function(sinc_kernel, 0);
    for (auto i : std::views::iota(0, 64))
        sinc_kernel[i] = sinc_kernel[i] * window[i];

    oscillator osc(48000);
    osc.freq(9000);
    osc.waveform = wave_tri;
    std::vector<clover_float> signal;
    signal.resize(64, 0);

    circular_buffer buffer{signal};
    for (auto i : std::views::iota(0, 64))
        buffer.tick(osc.tick());

    clover_float interpolated = interpolate_sinc(buffer, sinc_kernel);
    EXPECT_NEAR(interpolated, buffer[15], .000073);

    sinc_function(sinc_kernel, 1);
    hadamard_product(sinc_kernel, window);

    interpolated = interpolate_sinc(buffer, sinc_kernel);
    EXPECT_NEAR(interpolated, buffer[16], 0.0016);
}
