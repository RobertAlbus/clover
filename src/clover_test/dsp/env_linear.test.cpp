// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/dsp/env_linear.hpp"
#include "clover/float.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_env_linear, initializes_blank) {
    env_linear env;

    EXPECT_FLOAT_EQ(env.m_current_step, 0.f);
    EXPECT_FLOAT_EQ(env.m_target_step, 0.f);
    EXPECT_FLOAT_EQ(env.m_from, 0.f);
    EXPECT_FLOAT_EQ(env.m_to, 0.f);
}

TEST(dsp_env_linear, resets_from_to) {
    env_linear env;
    env.set(0, 10, 100);

    EXPECT_FLOAT_EQ(env.m_current_step, 0);
    EXPECT_FLOAT_EQ(env.m_target_step, 100);
    EXPECT_FLOAT_EQ(env.m_from, 0);
    EXPECT_FLOAT_EQ(env.m_to, 10);
}

TEST(dsp_env_linear, resets_to) {
    env_linear env;
    // initial conditions: halfway complete
    env.m_current_step = 50;
    env.m_target_step  = 100;
    env.m_from         = 0;
    env.m_to           = 10;

    env.set(9, 1000);
    EXPECT_FLOAT_EQ(env.m_current_step, 0);
    EXPECT_FLOAT_EQ(env.m_target_step, 1000);
    EXPECT_FLOAT_EQ(env.m_from, 5);
    EXPECT_FLOAT_EQ(env.m_to, 9);
}

TEST(dsp_env_linear, sets_to) {
    env_linear env;
    // initial conditions: halfway complete
    env.m_current_step = 50;
    env.m_target_step  = 100;
    env.m_from         = 0;
    env.m_to           = 10;

    env.set(9);
    EXPECT_FLOAT_EQ(env.m_current_step, 0);
    EXPECT_FLOAT_EQ(env.m_target_step, 50);
    EXPECT_FLOAT_EQ(env.m_from, 5);
    EXPECT_FLOAT_EQ(env.m_to, 9);
}

TEST(dsp_env_linear, correct_when_reset_from_and_to) {
    env_linear env;
    // initial conditions: halfway complete
    env.m_current_step = 123;
    env.m_target_step  = 1234;
    env.m_from         = 12345;
    env.m_to           = 123456;

    clover_float from     = 0;
    clover_float to       = 10;
    clover_float duration = 100;

    env.set(from, to, duration);
    for (auto n : std::views::iota(0, (int)duration)) {
        EXPECT_FLOAT_EQ(env.tick(), (n / duration) * to);
    }
    EXPECT_FLOAT_EQ(env.tick(), to);
}

TEST(dsp_env_linear, correct_when_reset_to) {
    env_linear env;
    // initial conditions: halfway complete
    env.m_current_step = 50;
    env.m_target_step  = 100;
    env.m_from         = 0;
    env.m_to           = 10;

    clover_float to       = 10;
    clover_float duration = 100;
    env.set(to, duration);

    for (auto n : std::views::iota(0, (int)duration)) {
        EXPECT_FLOAT_EQ(env.tick(), 5 + (n / duration) * (to - 5));
    }
}

TEST(dsp_env_linear, correct_when_set_to) {
    env_linear env;
    // initial conditions: halfway complete
    env.m_current_step = 50;
    env.m_target_step  = 100;
    env.m_from         = 0;
    env.m_to           = 10;

    clover_float new_to = 100;
    env.set(new_to);
    EXPECT_FLOAT_EQ(5, env.m_from);

    clover_float from_midpoint      = env.m_from;
    clover_float remaining_duration = env.m_target_step - env.m_current_step;

    for (auto n : std::views::iota(0, (int)remaining_duration)) {
        EXPECT_FLOAT_EQ(env.tick(), from_midpoint + (n / remaining_duration) * (new_to - from_midpoint));
    }
    EXPECT_FLOAT_EQ(env.tick(), new_to);
}
