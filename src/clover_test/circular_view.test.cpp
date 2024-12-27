// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/circular_view.hpp"
#include <cmath>
#include <cstdio>

#include "clover/float.hpp"
#include <gtest/gtest.h>

using namespace clover;

TEST(circular_view, instantiates_at_beginning) {
    auto data = std::views::iota(0, 10) |
                std::ranges::to<std::vector>();

    circular_view circle{data.begin(), data.end()};

    EXPECT_EQ(circle.m_begin, data.begin());
    EXPECT_EQ(circle.m_begin_from, data.begin());
    EXPECT_EQ(circle.m_sentinel, data.end());


    EXPECT_EQ(circle.begin(), data.begin());
}

TEST(circular_view, instantiates_from_position) {
    auto data = std::views::iota(0, 10) |
                std::views::transform([](int v) { return static_cast<clover_float>(v); }) |
                std::ranges::to<std::vector>();

    auto from = data.begin() + 4;

    circular_view circle{data.begin(), data.end(), from};

    EXPECT_EQ(circle.m_begin, data.begin());
    EXPECT_EQ(circle.m_begin_from, from);
    EXPECT_EQ(circle.m_sentinel, data.end());


    EXPECT_EQ(circle.begin().m_begin, data.begin());
    EXPECT_EQ(circle.begin(), from);
    EXPECT_EQ(circle.begin().m_sentinel, data.end());
}


TEST(circular_view, works_with_clover_float) {
    auto range = std::views::iota(0, 10);


    auto data = range |
                std::views::transform([](int v) { return static_cast<clover_float>(v); }) |
                std::ranges::to<std::vector>();

    circular_view circle{data.begin(), data.end()};

    for (auto n : range) {
        EXPECT_FLOAT_EQ(*circle.begin()[n], n);
    }

    auto from = data.begin() + 4;
    circular_view circle_offset{data.begin(), data.end(), from};

    for (auto n : range) {
        EXPECT_FLOAT_EQ(*circle_offset.begin()[n], n);
    }

    EXPECT_EQ(circle.m_begin, data.begin());
    EXPECT_EQ(circle.m_begin_from, from);
    EXPECT_EQ(circle.m_sentinel, data.end());


    EXPECT_EQ(circle.begin().m_begin, data.begin());
    EXPECT_EQ(circle.begin(), from);
    EXPECT_EQ(circle.begin().m_sentinel, data.end());
}