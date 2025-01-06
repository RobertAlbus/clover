// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/circular_buffer.hpp"
#include "clover/dsp/fractional_delay.hpp"
#include "clover/num.hpp"

using namespace clover;
using namespace dsp;

clover_float sin_signal(clover_float position, clover_float size) {
    clover_float delta = position / size;
    return std::sin(2 * (position / size) * num::pi);
}

TEST(dsp_fractional_delay, fdl_sinc_4) {
    std::shared_ptr<circular_buffer> buffer = std::make_shared<circular_buffer>(circular_buffer{5});

    for (auto i : std::views::iota(0, 5))
        buffer->tick(sin_signal(clover_float(i), 480));

    fdl_sinc sinc{buffer, 4, 1};

    sinc.delay(1);
    clover_float interpolated = sinc.calculate();
    EXPECT_FLOAT_EQ(interpolated, (*buffer)[1]);
    EXPECT_FLOAT_EQ(interpolated, sin_signal(3, 480));

    sinc.delay(1.5);
    interpolated = sinc.calculate();

    EXPECT_NEAR(interpolated, sin_signal(2.5f, 480), 0.005);

    sinc.delay(2);
    interpolated = sinc.calculate();
    EXPECT_FLOAT_EQ(interpolated, (*buffer)[2]);
    EXPECT_FLOAT_EQ(interpolated, sin_signal(2, 480));
}

TEST(dsp_fractional_delay, fdl_sinc_16) {
    //---------------------------------
    std::shared_ptr<circular_buffer> buffer = std::make_shared<circular_buffer>(circular_buffer{17});

    for (auto i : std::views::iota(0, 17))
        buffer->tick(sin_signal(clover_float(i), 480));

    fdl_sinc sinc{buffer, 16};

    /*
    0  | 0.00000 | 0.20791 |
    1  | 0.01308 | 0.19509 |
    2  | 0.02617 | 0.18223 |
    3  | 0.03925 | 0.16935 |
    4  | 0.05233 | 0.15643 |
    5  | 0.06540 | 0.14349 |
    6  | 0.07845 | 0.13052 |
    7  | 0.09150 | 0.11753 | <-               sinc.delay(7) * same
    8  | 0.10452 | 0.10452 | <- sin_signal(8) sinc.delay(8)
    9  | 0.11753 | 0.09150 | <- sin_signal(9)               * same
    10 | 0.13052 | 0.07845 |
    11 | 0.14349 | 0.06540 |
    12 | 0.15643 | 0.05233 |
    13 | 0.16935 | 0.03925 |
    14 | 0.18223 | 0.02617 |
    15 | 0.19509 | 0.01308 |
    16 | 0.20791 | 0.00000 |
    */

    sinc.delay(7);
    clover_float interpolated = sinc.calculate();
    EXPECT_NEAR(interpolated, (*buffer)[7], 0.00065);
    EXPECT_NEAR(interpolated, sin_signal(9, 480), 0.00065);

    sinc.delay(7.5);
    interpolated = sinc.calculate();
    EXPECT_NEAR(interpolated, sin_signal(8.5, 480), 0.0022);

    sinc.delay(8);
    interpolated = sinc.calculate();
    EXPECT_NEAR(interpolated, (*buffer)[8], 0.00058);
    EXPECT_NEAR(interpolated, sin_signal(8, 480), 0.00058);
}

TEST(dsp_fractional_delay, fdl_sinc_64) {
    std::shared_ptr<circular_buffer> buffer = std::make_shared<circular_buffer>(circular_buffer{65});

    for (auto i : std::views::iota(0, 65))
        buffer->tick(sin_signal(clover_float(i), 480));

    fdl_sinc sinc{buffer, 64};

    sinc.delay(31);
    clover_float interpolated = sinc.calculate();
    EXPECT_NEAR(interpolated, (*buffer)[31], 0.00014);
    EXPECT_NEAR(interpolated, sin_signal(33, 480), 0.00014);

    sinc.delay(31.5);
    interpolated = sinc.calculate();
    EXPECT_NEAR(interpolated, sin_signal(32.5, 480), 0.00188);

    sinc.delay(32);
    interpolated = sinc.calculate();
    EXPECT_NEAR(interpolated, (*buffer)[32], 0.00013);
    EXPECT_NEAR(interpolated, sin_signal(32, 480), 0.00013);
}

TEST(dsp_fractional_delay, langrange) {
    fdl_lagrange lagrange{65};
    for (auto i : std::views::iota(0, 65))
        lagrange.tick(sin_signal(clover_float(i), 480));

    clover_float interpolated = lagrange.at(31);
    EXPECT_FLOAT_EQ(interpolated, lagrange.m_buffer[31]);
    EXPECT_FLOAT_EQ(interpolated, sin_signal(33, 480));

    interpolated = lagrange.at(31.5);
    EXPECT_NEAR(interpolated, sin_signal(32.5, 480), 0.00671);

    interpolated = lagrange.at(32);
    EXPECT_FLOAT_EQ(interpolated, lagrange.m_buffer[32]);
    EXPECT_FLOAT_EQ(interpolated, sin_signal(32, 480));
}

TEST(dsp_fractional_delay, langrange_2) {
    fdl_lagrange_2 lagrange{130};
    for (auto i : std::views::iota(0, 65)) {
        clover_float signal = sin_signal(clover_float(i), 480);
        lagrange.tick(signal, signal);
    }

    auto interpolated = lagrange.at(31);
    EXPECT_FLOAT_EQ(interpolated.first, lagrange.m_buffer[31].first);
    EXPECT_FLOAT_EQ(interpolated.second, lagrange.m_buffer[31].second);
    EXPECT_FLOAT_EQ(interpolated.first, sin_signal(33, 480));
    EXPECT_FLOAT_EQ(interpolated.second, sin_signal(33, 480));

    interpolated = lagrange.at(31.5);
    EXPECT_NEAR(interpolated.first, sin_signal(32.5, 480), 0.00671);
    EXPECT_NEAR(interpolated.second, sin_signal(32.5, 480), 0.00671);

    interpolated = lagrange.at(32);
    EXPECT_FLOAT_EQ(interpolated.first, lagrange.m_buffer[32].first);
    EXPECT_FLOAT_EQ(interpolated.second, lagrange.m_buffer[32].second);
    EXPECT_FLOAT_EQ(interpolated.first, sin_signal(32, 480));
    EXPECT_FLOAT_EQ(interpolated.second, sin_signal(32, 480));
}
