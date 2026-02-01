// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <gtest/gtest.h>

#include "clover/dsp/phase_increment_tracker.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_phase_increment_tracker, initializes_by_freq) {
    float fs = 48000.f;

    float freq = 12345.99999f;
    auto pit   = phase_increment_tracker::for_freq(fs, freq);

    EXPECT_FLOAT_EQ(pit.freq(), freq);
    EXPECT_FLOAT_EQ(pit.phase(), 0.f);
    EXPECT_FLOAT_EQ(pit.phase_offset(), 0.f);
    EXPECT_FLOAT_EQ(pit.period(), fs / freq);
    EXPECT_FLOAT_EQ(pit.m_domain, num::pi_x2);

    float neg_freq = -384.f;
    auto pit_neg   = phase_increment_tracker::for_freq(fs, neg_freq);

    EXPECT_FLOAT_EQ(pit_neg.freq(), neg_freq);
    EXPECT_FLOAT_EQ(pit_neg.period(), fs / neg_freq);
}

TEST(dsp_phase_increment_tracker, initializes_by_period) {
    float fs = 48000.f;

    float period = 12345.99999f;
    auto pit     = phase_increment_tracker::for_period(fs, period);

    EXPECT_FLOAT_EQ(pit.freq(), fs / period);
    EXPECT_FLOAT_EQ(pit.phase(), 0.f);
    EXPECT_FLOAT_EQ(pit.phase_offset(), 0.f);
    EXPECT_FLOAT_EQ(pit.period(), period);
    EXPECT_FLOAT_EQ(pit.m_domain, num::pi_x2);

    float neg_period = -125.f;
    auto pit_neg     = phase_increment_tracker::for_period(fs, neg_period);

    EXPECT_FLOAT_EQ(pit_neg.period(), neg_period);
    EXPECT_FLOAT_EQ(pit_neg.freq(), fs / neg_period);
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

    pit.freq(24001);
    EXPECT_EQ(pit.freq(), 24000);

    pit.freq(-111.f);
    EXPECT_FLOAT_EQ(pit.freq(), -111.f);
    EXPECT_FLOAT_EQ(pit.period(), fs / -111.f);

    pit.freq(-30000.f);
    EXPECT_FLOAT_EQ(pit.freq(), -24000.f);
}

TEST(dsp_phase_increment_tracker, period_set_correctly) {
    float fs = 48000.f;

    auto pit = phase_increment_tracker::for_period(fs, 100);

    pit.period(1000.f);
    EXPECT_FLOAT_EQ(pit.period(), 1000.f);
    EXPECT_FLOAT_EQ(pit.freq(), fs / 1000.f);

    pit.period(-1000.f);
    EXPECT_FLOAT_EQ(pit.period(), -1000.f);
    EXPECT_FLOAT_EQ(pit.freq(), fs / -1000.f);
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

TEST(dsp_phase_increment_tracker, decrements_correctly_with_negative_freq) {
    float freq = -384.f;
    float fs   = 48000.f;

    auto pit = phase_increment_tracker::for_freq(fs, freq);

    pit.tick();
    float expected_decrement = std::abs(freq) / (fs / pit.m_domain);
    EXPECT_FLOAT_EQ(pit.phase(), pit.m_domain - expected_decrement);
}

TEST(dsp_phase_increment_tracker, completes_backward_cycle) {
    float period = -125.f;
    float fs     = 48000.f;

    auto pit = phase_increment_tracker::for_period(fs, period);

    for (int i = 0; i < 125; ++i) {
        pit.tick();
    }
    EXPECT_FLOAT_EQ(pit.phase(), 0.f);
}
