// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/audio_buffer.hpp"
#include <cmath>
#include <cstdio>

#include <gtest/gtest.h>

#include "clover/audio_buffer.hpp"

using namespace clover;

TEST(audio_buffer, normlize) {
    audio_buffer buffer;
    buffer.data.emplace_back(-2);
    buffer.data.emplace_back(1);

    normalize_audio_buffer(buffer);

    EXPECT_FLOAT_EQ(buffer.data[0], -1);
    EXPECT_FLOAT_EQ(buffer.data[1], 0.5);
}
