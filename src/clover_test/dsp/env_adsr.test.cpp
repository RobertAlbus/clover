// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/dsp/env_adsr.hpp"
#include "clover/float.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_env_adsr, initializes_blank) {
    env_adsr env;

    EXPECT_FLOAT_EQ(env.attack(), 0.f);
    EXPECT_FLOAT_EQ(env.decay(), 0.f);
    EXPECT_FLOAT_EQ(env.sustain(), 0.f);
    EXPECT_FLOAT_EQ(env.release(), 0.f);

    EXPECT_FALSE(env.is_active());

    EXPECT_FLOAT_EQ(env.tick(), 0.f);
}

TEST(dsp_env_adsr, sets_correctly) {
    env_adsr env;

    env.set(10, 20, 0.5, 30.);

    EXPECT_FLOAT_EQ(env.attack(), 10);
    EXPECT_FLOAT_EQ(env.decay(), 20);
    EXPECT_FLOAT_EQ(env.sustain(), 0.5);
    EXPECT_FLOAT_EQ(env.release(), 30);

    env.attack(100);
    EXPECT_FLOAT_EQ(env.attack(), 100);

    env.decay(200);
    EXPECT_FLOAT_EQ(env.decay(), 200);

    env.sustain(0.);
    EXPECT_FLOAT_EQ(env.sustain(), 0.);

    env.release(300);
    EXPECT_FLOAT_EQ(env.release(), 300);
}

TEST(dsp_env_adsr, key_must_be_on) {
    clover_float a = 10;
    clover_float d = 10;
    clover_float s = 0.5;
    clover_float r = 10;

    env_adsr env;
    env.set(a, d, s, r);

    for (auto x : std::views::iota(0, 10))
        EXPECT_EQ(env.tick(), 0);
}

TEST(dsp_env_adsr, runs_correctly) {
    clover_float a = 10;
    clover_float d = 10;
    clover_float s = 0.5;
    clover_float r = 10;

    env_adsr env;
    env.set(a, d, s, r);

    EXPECT_EQ(env.m_state, env_adsr::state::init);

    env.key_on();
    EXPECT_EQ(env.m_state, env_adsr::state::attack);
    EXPECT_FLOAT_EQ(env.tick(), 0.1);
    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.3);
    EXPECT_FLOAT_EQ(env.tick(), 0.4);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);
    EXPECT_FLOAT_EQ(env.tick(), 0.6);
    EXPECT_FLOAT_EQ(env.tick(), 0.7);
    EXPECT_FLOAT_EQ(env.tick(), 0.8);
    EXPECT_FLOAT_EQ(env.tick(), 0.9);

    EXPECT_FLOAT_EQ(env.tick(), 1.0);
    EXPECT_EQ(env.m_state, env_adsr::state::decay);

    EXPECT_FLOAT_EQ(env.tick(), 0.95);
    EXPECT_FLOAT_EQ(env.tick(), 0.90);
    EXPECT_FLOAT_EQ(env.tick(), 0.85);
    EXPECT_FLOAT_EQ(env.tick(), 0.80);
    EXPECT_FLOAT_EQ(env.tick(), 0.75);
    EXPECT_FLOAT_EQ(env.tick(), 0.70);
    EXPECT_FLOAT_EQ(env.tick(), 0.65);
    EXPECT_FLOAT_EQ(env.tick(), 0.60);
    EXPECT_FLOAT_EQ(env.tick(), 0.55);
    EXPECT_FLOAT_EQ(env.tick(), 0.50);

    EXPECT_EQ(env.m_state, env_adsr::state::sustain);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);

    env.key_off();
    EXPECT_EQ(env.m_state, env_adsr::state::release);

    EXPECT_FLOAT_EQ(env.tick(), 0.45);
    EXPECT_FLOAT_EQ(env.tick(), 0.40);
    EXPECT_FLOAT_EQ(env.tick(), 0.35);
    EXPECT_FLOAT_EQ(env.tick(), 0.30);
    EXPECT_FLOAT_EQ(env.tick(), 0.25);
    EXPECT_FLOAT_EQ(env.tick(), 0.20);
    EXPECT_FLOAT_EQ(env.tick(), 0.15);
    EXPECT_FLOAT_EQ(env.tick(), 0.10);
    EXPECT_FLOAT_EQ(env.tick(), 0.05);

    EXPECT_EQ(env.m_state, env_adsr::state::complete);

    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
}

TEST(dsp_env_adsr, reruns_correctly) {
    clover_float a = 10;
    clover_float d = 10;
    clover_float s = 0.5;
    clover_float r = 10;

    env_adsr env;
    env.set(a, d, s, r);

    env.key_on();
    EXPECT_FLOAT_EQ(env.tick(), 0.1);
    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.3);

    env.key_on();
    EXPECT_EQ(env.m_state, env_adsr::state::attack);
    EXPECT_FLOAT_EQ(env.tick(), 0.1);
    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.3);
}

TEST(dsp_env_adsr, mid_attack_update) {
    env_adsr env;
    env.set(10, 10, 0.5, 10);

    env.key_on();
    EXPECT_FLOAT_EQ(env.tick(), 0.1);
    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.3);
    EXPECT_FLOAT_EQ(env.tick(), 0.4);

    env.attack(15);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);
    EXPECT_FLOAT_EQ(env.tick(), 0.55);
    EXPECT_FLOAT_EQ(env.tick(), 0.60);
    EXPECT_FLOAT_EQ(env.tick(), 0.65);
    EXPECT_FLOAT_EQ(env.tick(), 0.70);
    EXPECT_FLOAT_EQ(env.tick(), 0.75);
    EXPECT_FLOAT_EQ(env.tick(), 0.80);
    EXPECT_FLOAT_EQ(env.tick(), 0.85);
    EXPECT_FLOAT_EQ(env.tick(), 0.90);
    EXPECT_FLOAT_EQ(env.tick(), 0.95);

    EXPECT_FLOAT_EQ(env.tick(), 1);
}

TEST(dsp_env_adsr, mid_decay_update) {
    env_adsr env;
    env.set(10, 10, 0.5, 10);

    // half way through decay
    env.key_on();
    for (auto _ : std::views::iota(0, 14))
        env.tick();

    env.decay(15);
    EXPECT_FLOAT_EQ(env.tick(), 0.75);
    EXPECT_FLOAT_EQ(env.tick(), 0.725);
    EXPECT_FLOAT_EQ(env.tick(), 0.700);
    EXPECT_FLOAT_EQ(env.tick(), 0.675);
    EXPECT_FLOAT_EQ(env.tick(), 0.650);
    EXPECT_FLOAT_EQ(env.tick(), 0.625);
    EXPECT_FLOAT_EQ(env.tick(), 0.600);
    EXPECT_FLOAT_EQ(env.tick(), 0.575);
    EXPECT_FLOAT_EQ(env.tick(), 0.550);
    EXPECT_FLOAT_EQ(env.tick(), 0.525);

    // reached sustain
    EXPECT_FLOAT_EQ(env.tick(), 0.500);
    EXPECT_FLOAT_EQ(env.tick(), 0.500);
    EXPECT_FLOAT_EQ(env.tick(), 0.500);
    EXPECT_FLOAT_EQ(env.tick(), 0.500);
}

TEST(dsp_env_adsr, mid_sustain_update) {
    env_adsr env;
    env.set(5, 5, 0.5, 10);

    // get to sustain
    env.key_on();
    for (auto _ : std::views::iota(0, 15))
        env.tick();

    EXPECT_FLOAT_EQ(env.tick(), 0.5);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);
    EXPECT_FLOAT_EQ(env.tick(), 0.5);

    env.sustain(0.1);

    EXPECT_FLOAT_EQ(env.tick(), 0.1);
    EXPECT_FLOAT_EQ(env.tick(), 0.1);
    EXPECT_FLOAT_EQ(env.tick(), 0.1);
}

TEST(dsp_env_adsr, mid_release_update) {
    env_adsr env;
    env.set(2, 2, 0.5, 10);

    // get to sustain
    env.key_on();
    for (auto _ : std::views::iota(0, 5))
        env.tick();

    EXPECT_FLOAT_EQ(env.tick(), 0.50);

    env.key_off();
    EXPECT_FLOAT_EQ(env.tick(), 0.45);
    EXPECT_FLOAT_EQ(env.tick(), 0.40);
    EXPECT_FLOAT_EQ(env.tick(), 0.35);
    EXPECT_FLOAT_EQ(env.tick(), 0.30);
    // half way through release

    env.release(15);
    EXPECT_FLOAT_EQ(env.tick(), 0.25);
    EXPECT_FLOAT_EQ(env.tick(), 0.225);
    EXPECT_FLOAT_EQ(env.tick(), 0.200);
    EXPECT_FLOAT_EQ(env.tick(), 0.175);
    EXPECT_FLOAT_EQ(env.tick(), 0.150);
    EXPECT_FLOAT_EQ(env.tick(), 0.125);
    EXPECT_FLOAT_EQ(env.tick(), 0.100);
    EXPECT_FLOAT_EQ(env.tick(), 0.075);
    EXPECT_FLOAT_EQ(env.tick(), 0.050);
    EXPECT_FLOAT_EQ(env.tick(), 0.025);

    // complete
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
}

TEST(dsp_env_adsr, key_on_retriggers) {
    env_adsr env;
    env.set(5, 5, 1, 10);

    env.key_on();
    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.4);
    EXPECT_FLOAT_EQ(env.tick(), 0.6);

    env.key_on();
    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.4);
    EXPECT_FLOAT_EQ(env.tick(), 0.6);
}

TEST(dsp_env_adsr, key_off_only_once) {
    env_adsr env;
    env.set(5, 5, 1, 10);

    // get to sustain
    env.key_on();
    for (auto _ : std::views::iota(0, 15))
        env.tick();

    env.key_off();
    EXPECT_FLOAT_EQ(env.tick(), 0.9);
    EXPECT_FLOAT_EQ(env.tick(), 0.8);

    env.key_off();
    EXPECT_EQ(env.m_state, env_adsr::state::release);
    EXPECT_FLOAT_EQ(env.tick(), 0.7);
    EXPECT_FLOAT_EQ(env.tick(), 0.6);
}

TEST(dsp_env_adsr, attack_gain_zero) {
    env_adsr env;
    env.set(5, 0, 5, 1, 5);

    // get to sustain
    env.key_on();
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);
    EXPECT_FLOAT_EQ(env.tick(), 0);

    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.4);
    EXPECT_FLOAT_EQ(env.tick(), 0.6);
    EXPECT_FLOAT_EQ(env.tick(), 0.8);
    EXPECT_FLOAT_EQ(env.tick(), 1.0);

    env.key_off();

    EXPECT_FLOAT_EQ(env.tick(), 0.8);
    EXPECT_FLOAT_EQ(env.tick(), 0.6);
    EXPECT_FLOAT_EQ(env.tick(), 0.4);
    EXPECT_FLOAT_EQ(env.tick(), 0.2);
    EXPECT_FLOAT_EQ(env.tick(), 0.0);
}