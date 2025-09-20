// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstdio>
#include <gtest/gtest.h>

#include "clover/dsp/phase_increment_tracker.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_phase_increment_tracker, initializes_by_freq) {
    float freq = 12345.99999f;
    float fs   = 48000.f;

    auto pit = phase_increment_tracker::for_freq(fs, freq);

    EXPECT_FLOAT_EQ(pit.freq(), freq);
    EXPECT_FLOAT_EQ(pit.phase(), 0.f);
    EXPECT_FLOAT_EQ(pit.phase_offset(), 0.f);
    EXPECT_FLOAT_EQ(pit.period(), fs / freq);
    EXPECT_FLOAT_EQ(pit.m_domain, num::pi_x2);
}

TEST(dsp_phase_increment_tracker, initializes_by_period) {
    float period = 12345.99999f;
    float fs     = 48000.f;

    auto pit = phase_increment_tracker::for_period(fs, period);

    EXPECT_FLOAT_EQ(pit.freq(), fs / period);
    EXPECT_FLOAT_EQ(pit.phase(), 0.f);
    EXPECT_FLOAT_EQ(pit.phase_offset(), 0.f);
    EXPECT_FLOAT_EQ(pit.period(), period);
    EXPECT_FLOAT_EQ(pit.m_domain, num::pi_x2);
}

TEST(dsp_phase_increment_tracker, increments_correctly) {
    float freq = 384.f;
    float fs   = 48000.f;

    auto pit = phase_increment_tracker::for_freq(fs, freq);

    for (int i = 0; i < (int)(fs / freq); ++i) {
        EXPECT_FLOAT_EQ(pit.phase(), (freq / (fs / pit.m_domain)) * i);
        pit.tick();
    }
    EXPECT_FLOAT_EQ(pit.phase(), 0.f);
}

TEST(dsp_phase_increment_tracker, frequency_set_correctly) {
    float freq = 111;
    float fs   = 48000.f;

    auto pit = phase_increment_tracker::for_freq(fs, 1);

    pit.freq(freq);
    EXPECT_EQ(pit.freq(), freq);

    pit.freq(-3);
    EXPECT_EQ(pit.freq(), 0);

    pit.freq(24001);
    EXPECT_EQ(pit.freq(), 24000);
}

TEST(dsp_phase_increment_tracker, phase_set_correctly) {
    float freq = 111;
    float fs   = 48000.f;

    auto pit = phase_increment_tracker::for_freq(fs, 1);

    EXPECT_EQ(pit.phase(), 0);

    pit.phase(pit.m_domain);
    EXPECT_EQ(pit.phase(), 0);

    pit.phase(1);
    EXPECT_EQ(pit.phase(), 1);

    pit.phase(-1);
    EXPECT_EQ(pit.phase(), pit.m_domain - 1);

    pit.phase(pit.m_domain + 1);
    EXPECT_EQ(pit.phase(), 1);
}

TEST(dsp_phase_increment_tracker, phase_offset_set_correctly) {
    float fs = 48000.f;

    auto pit = phase_increment_tracker::for_freq(fs, 1.f);

    pit.phase_offset(1);
    EXPECT_EQ(pit.phase_offset(), 1);
    EXPECT_EQ(pit.phase(), 1);

    pit.phase(1);
    EXPECT_EQ(pit.phase(), 2);

    pit.phase(pit.m_domain);
    EXPECT_EQ(pit.phase(), 1);

    pit.phase_offset(-1);
    pit.phase(0);
    EXPECT_EQ(pit.phase(), pit.m_domain - 1);
}
