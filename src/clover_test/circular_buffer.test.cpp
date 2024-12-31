// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>

#include "clover/circular_buffer.hpp"
#include "clover/float.hpp"
#include <gtest/gtest.h>
#include <ranges>

using namespace clover;
using namespace dsp;

TEST(circular_buffer, reads_and_writes) {
    std::vector<clover_float> audio_data;
    audio_data.resize(10, 0);

    circular_buffer buffer = {audio_data};

    for (auto i : std::views::iota(0, 10)) {
        buffer.tick(static_cast<clover_float>(i));
        std::cout << "tick " << buffer.m_current << std::endl;
        for (auto x : audio_data)
            std::cout << x << std::endl;
    }

    EXPECT_FLOAT_EQ(buffer[0], 9);
    EXPECT_FLOAT_EQ(buffer[1], 8);
    EXPECT_FLOAT_EQ(buffer[2], 7);
    EXPECT_FLOAT_EQ(buffer[3], 6);
    EXPECT_FLOAT_EQ(buffer[4], 5);
    EXPECT_FLOAT_EQ(buffer[5], 4);
    EXPECT_FLOAT_EQ(buffer[6], 3);
    EXPECT_FLOAT_EQ(buffer[7], 2);
    EXPECT_FLOAT_EQ(buffer[8], 1);
    EXPECT_FLOAT_EQ(buffer[9], 0);

    for (auto i : std::views::iota(100, 105)) {
        buffer.tick(static_cast<clover_float>(i));
    }

    EXPECT_FLOAT_EQ(buffer[0], 104);
    EXPECT_FLOAT_EQ(buffer[1], 103);
    EXPECT_FLOAT_EQ(buffer[2], 102);
    EXPECT_FLOAT_EQ(buffer[3], 101);
    EXPECT_FLOAT_EQ(buffer[4], 100);
    EXPECT_FLOAT_EQ(buffer[5], 9);
    EXPECT_FLOAT_EQ(buffer[6], 8);
    EXPECT_FLOAT_EQ(buffer[7], 7);
    EXPECT_FLOAT_EQ(buffer[8], 6);
    EXPECT_FLOAT_EQ(buffer[9], 5);
}

TEST(circular_buffer, throw_when_out_of_range) {
    std::vector<clover_float> audio_data;
    audio_data.resize(20, 0);

    circular_buffer buffer = {audio_data};

    EXPECT_ANY_THROW({ buffer[20]; });
}

TEST(circular_buffer, size) {
    std::vector<clover_float> audio_data;
    audio_data.resize(20, 0);

    circular_buffer buffer = {audio_data};
    EXPECT_EQ(buffer.size(), audio_data.size());
}
