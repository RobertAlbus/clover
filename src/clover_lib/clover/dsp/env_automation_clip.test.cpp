// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <stdexcept>

#include <gtest/gtest.h>

#include "clover/dsp/env_automation_clip.hpp"
using namespace clover;
using namespace dsp;

TEST(dsp_env_automation_clip, initializes_with_list) {
    std::map<int64_t, clover_float> points{{0, 0}, {10, 1}, {20, 0}};
    EXPECT_NO_THROW({
        env_automation_clip automation_clip_from_list({{0, 0}, {1, 1}});
        env_automation_clip automation_clip_from_map(points);
    });

    env_automation_clip env(points);
    EXPECT_FLOAT_EQ(env.m_section_size, 10);
    EXPECT_EQ(env.m_current_point, env.m_points.cbegin());
}

TEST(dsp_env_automation_clip, throws_when_not_enough_points) {
    EXPECT_THROW(env_automation_clip({{0, 0}}), std::invalid_argument);

    std::map<int64_t, clover_float> no_points{};
    EXPECT_THROW(env_automation_clip{no_points}, std::invalid_argument);
}

TEST(dsp_env_automation_clip, throws_when_starts_after_zero) {
    EXPECT_THROW(env_automation_clip({{10, 0}, {100, 1}}), std::invalid_argument);
}

TEST(dsp_env_automation_clip, is_correct) {
    std::map<int64_t, clover_float> points{{0, 0}, {10, 10}, {20, 0}};

    env_automation_clip env(points);

    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 1);
    EXPECT_FLOAT_EQ(env.tick(), 2);
    EXPECT_FLOAT_EQ(env.tick(), 3);
    EXPECT_FLOAT_EQ(env.tick(), 4);
    EXPECT_FLOAT_EQ(env.tick(), 5);
    EXPECT_FLOAT_EQ(env.tick(), 6);
    EXPECT_FLOAT_EQ(env.tick(), 7);
    EXPECT_FLOAT_EQ(env.tick(), 8);
    EXPECT_FLOAT_EQ(env.tick(), 9);
    EXPECT_FLOAT_EQ(env.tick(), 10);
    EXPECT_FLOAT_EQ(env.tick(), 9);
    EXPECT_FLOAT_EQ(env.tick(), 8);
    EXPECT_FLOAT_EQ(env.tick(), 7);
    EXPECT_FLOAT_EQ(env.tick(), 6);
    EXPECT_FLOAT_EQ(env.tick(), 5);
    EXPECT_FLOAT_EQ(env.tick(), 4);
    EXPECT_FLOAT_EQ(env.tick(), 3);
    EXPECT_FLOAT_EQ(env.tick(), 2);
    EXPECT_FLOAT_EQ(env.tick(), 1);
    EXPECT_FLOAT_EQ(env.tick(), 0);

    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
}

TEST(dsp_env_automation_clip, sorts_points) {
    std::map<int64_t, clover_float> points{{0, 0}, {2, 0}, {1, 1}, {3, 1}};

    env_automation_clip env(points);

    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 1);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 1);
}
