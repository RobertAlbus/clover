// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>

#include <gtest/gtest.h>
#include <unordered_map>

#include "clover/dsp/wave.hpp"
#include "clover/float.hpp"
#include "clover/num.hpp"

using namespace clover;
using namespace dsp;

// SINE
TEST(dsp_wave_sin, range_is_correct) {
    constexpr clover_float delta_t = num::pi_x2 / 10000;
    for (int i = 0; i < 10000; ++i) {
        clover_float signal = clover::dsp::wave_sine(delta_t * static_cast<clover_float>(i));
        EXPECT_LE(signal, 1);
        EXPECT_GE(signal, -1);
    }
}

TEST(dsp_wave_sin, value_is_correct) {
    constexpr clover_float delta_t = num::pi_x2 / 10000;
    for (int i = 0; i < 10000; ++i) {
        clover_float theta  = delta_t * static_cast<clover_float>(i);
        clover_float signal = clover::dsp::wave_sine(theta);
        EXPECT_FLOAT_EQ(signal, std::sin(theta));
    }
}

// SQUARE
TEST(dsp_wave_square, range_is_correct) {
    constexpr clover_float delta_t = num::pi_x2 / 10000;
    for (int i = 0; i < 10000; ++i) {
        clover_float signal = clover::dsp::wave_square(delta_t * static_cast<clover_float>(i));
        EXPECT_LE(signal, 1);
        EXPECT_GE(signal, -1);
    }
}

TEST(dsp_wave_square, value_is_correct) {
    constexpr clover_float delta_t = num::pi_x2 / 10000;

    for (int i = 0; i < 10000; ++i) {
        clover_float theta    = delta_t * static_cast<clover_float>(i);
        clover_float signal   = clover::dsp::wave_square(theta);
        clover_float expected = theta < num::pi ? 1.f : 0.f;
        EXPECT_FLOAT_EQ(signal, expected);
    }
}

// SAW
TEST(dsp_wave_saw, range_is_correct) {
    constexpr clover_float delta_t = num::pi_x2 / 10000;
    for (int i = 0; i < 10000; ++i) {
        clover_float signal = clover::dsp::wave_saw(delta_t * static_cast<clover_float>(i));
        EXPECT_LE(signal, 1);
        EXPECT_GE(signal, -1);
    }
}

TEST(dsp_wave_saw, value_is_correct) {
    constexpr clover_float delta_t = num::pi_x2 / 10000;

    for (int i = 0; i < 10000; ++i) {
        clover_float theta    = delta_t * static_cast<clover_float>(i);
        clover_float signal   = clover::dsp::wave_saw(theta);
        clover_float expected = std::lerp(1.f, -1.f, theta / num::pi_x2);
        EXPECT_FLOAT_EQ(signal, expected);
    }
}

// TRI
TEST(dsp_wave_tri, range_is_correct) {
    constexpr clover_float delta_t = num::pi_x2 / 100000;
    for (int i = 0; i < 100000; ++i) {
        clover_float signal = clover::dsp::wave_tri(delta_t * static_cast<clover_float>(i));
        EXPECT_LE(signal, 1);
        EXPECT_GE(signal, -1);
    }
}

TEST(dsp_wave_tri, value_is_correct) {
    std::unordered_map<clover_float, clover_float> thetas{
            {0.f, 0.f},
            {num::pi * 0.5, 1.f},
            {num::pi, 0.f},
            {num::pi * 1.5f, -1.f},
    };
    for (auto& theta : thetas) {
        EXPECT_FLOAT_EQ(clover::dsp::wave_tri(theta.first), theta.second);
    }
}

// NOISE WHITE
TEST(dsp_wave_noise, range_is_correct) {
    for (int i = 0; i < 10000; ++i) {
        clover_float signal = clover::dsp::wave_noise(0);
        EXPECT_LE(signal, 1);
        EXPECT_GE(signal, -1);
    }
}
