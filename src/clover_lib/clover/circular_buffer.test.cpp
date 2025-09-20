// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>

#include "clover/circular_buffer.hpp"
#include <gtest/gtest.h>
#include <ranges>

using namespace clover;
using namespace dsp;

TEST(circular_buffer, reads_and_writes) {
    circular_buffer buffer{10};

    for (auto i : std::views::iota(0, 10)) {
        buffer.tick(static_cast<float>(i));
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
        buffer.tick(static_cast<float>(i));
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
    circular_buffer buffer{20};

    EXPECT_ANY_THROW({ buffer[20]; });
}

TEST(circular_buffer, length) {
    circular_buffer buffer{20};
    EXPECT_EQ(buffer.length(), 20);
}

TEST(circular_buffer_2, reads_and_writes) {
    circular_buffer_2 buffer{10};

    for (auto i : std::views::iota(0, 10)) {
        auto i_f = float(i * 2);
        buffer.tick(i_f, i_f + 1);
    }

    EXPECT_FLOAT_EQ(buffer[0].second, 19);
    EXPECT_FLOAT_EQ(buffer[0].first, 18);
    EXPECT_FLOAT_EQ(buffer[1].second, 17);
    EXPECT_FLOAT_EQ(buffer[1].first, 16);
    EXPECT_FLOAT_EQ(buffer[2].second, 15);
    EXPECT_FLOAT_EQ(buffer[2].first, 14);
    EXPECT_FLOAT_EQ(buffer[3].second, 13);
    EXPECT_FLOAT_EQ(buffer[3].first, 12);
    EXPECT_FLOAT_EQ(buffer[4].second, 11);
    EXPECT_FLOAT_EQ(buffer[4].first, 10);
    EXPECT_FLOAT_EQ(buffer[5].second, 9);
    EXPECT_FLOAT_EQ(buffer[5].first, 8);
    EXPECT_FLOAT_EQ(buffer[6].second, 7);
    EXPECT_FLOAT_EQ(buffer[6].first, 6);
    EXPECT_FLOAT_EQ(buffer[7].second, 5);
    EXPECT_FLOAT_EQ(buffer[7].first, 4);
    EXPECT_FLOAT_EQ(buffer[8].second, 3);
    EXPECT_FLOAT_EQ(buffer[8].first, 2);
    EXPECT_FLOAT_EQ(buffer[9].second, 1);
    EXPECT_FLOAT_EQ(buffer[9].first, 0);

    for (auto i : std::views::iota(0, 5)) {
        auto i_f = 100 + static_cast<float>(i * 2);
        buffer.tick(i_f, i_f + 1);
    }

    EXPECT_FLOAT_EQ(buffer[0].second, 109);
    EXPECT_FLOAT_EQ(buffer[0].first, 108);
    EXPECT_FLOAT_EQ(buffer[1].second, 107);
    EXPECT_FLOAT_EQ(buffer[1].first, 106);
    EXPECT_FLOAT_EQ(buffer[2].second, 105);
    EXPECT_FLOAT_EQ(buffer[2].first, 104);
    EXPECT_FLOAT_EQ(buffer[3].second, 103);
    EXPECT_FLOAT_EQ(buffer[3].first, 102);
    EXPECT_FLOAT_EQ(buffer[4].second, 101);
    EXPECT_FLOAT_EQ(buffer[4].first, 100);
    EXPECT_FLOAT_EQ(buffer[5].second, 19);
    EXPECT_FLOAT_EQ(buffer[5].first, 18);
    EXPECT_FLOAT_EQ(buffer[6].second, 17);
    EXPECT_FLOAT_EQ(buffer[6].first, 16);
    EXPECT_FLOAT_EQ(buffer[7].second, 15);
    EXPECT_FLOAT_EQ(buffer[7].first, 14);
    EXPECT_FLOAT_EQ(buffer[8].second, 13);
    EXPECT_FLOAT_EQ(buffer[8].first, 12);
    EXPECT_FLOAT_EQ(buffer[9].second, 11);
    EXPECT_FLOAT_EQ(buffer[9].first, 10);
}

TEST(circular_buffer_2, throw_when_out_of_range) {
    circular_buffer_2 buffer{10};

    EXPECT_ANY_THROW({ buffer[10]; });
}

TEST(circular_buffer_2, length) {
    circular_buffer_2 buffer{20};
    EXPECT_EQ(buffer.length(), 20);
}