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

TEST(circular_buffer, instantiates) {
    std::vector<clover_float> audio_data;
    audio_data.resize(20, 0);

    circular_buffer buffer{audio_data.rbegin(), audio_data.rend()};

    EXPECT_EQ(audio_data.rbegin(), buffer.m_rbegin);
    EXPECT_EQ(audio_data.rbegin(), buffer.m_current);
    EXPECT_EQ(audio_data.rend(), buffer.m_rend);

    buffer = {audio_data};

    EXPECT_EQ(audio_data.rbegin(), buffer.m_rbegin);
    EXPECT_EQ(audio_data.rbegin(), buffer.m_current);
    EXPECT_EQ(audio_data.rend(), buffer.m_rend);
}

TEST(circular_buffer, reads_and_writes) {
    std::vector<clover_float> audio_data;
    audio_data.resize(20, 0);

    circular_buffer buffer = {audio_data.rbegin(), audio_data.rend()};

    for (auto i : std::views::iota(0, 20))
        buffer.tick(static_cast<clover_float>(i));

    for (auto i : std::views::iota(0, 20)) {
        EXPECT_FLOAT_EQ(audio_data[i], static_cast<clover_float>(19 - i));
        EXPECT_FLOAT_EQ(buffer[i], i);
    }
}

TEST(circular_buffer, throw_when_out_of_range) {
    std::vector<clover_float> audio_data;
    audio_data.resize(20, 0);

    circular_buffer buffer = {audio_data.rbegin(), audio_data.rend()};

    EXPECT_ANY_THROW({ buffer[20]; });
}