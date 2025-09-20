// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>

#include <gtest/gtest.h>

#include "clover/dsp/oscillator.hpp"
#include "clover/float.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_oscillator, initializes_blank) {
    clover_float fs = 48000.f;

    oscillator osc(fs);
    EXPECT_FLOAT_EQ(osc.freq(), 0.f);
    EXPECT_FLOAT_EQ(osc.phase(), 0.f);
    EXPECT_FLOAT_EQ(osc.phase_offset(), 0.f);
    EXPECT_FLOAT_EQ(osc.period(), 0.f);
    EXPECT_FLOAT_EQ(osc.note(), 0.f);
}

TEST(dsp_oscillator, sets_correctly) {
    clover_float fs = 48000.f;

    oscillator osc(fs);

    clover_float freq          = 100.f;
    clover_float initial_phase = 0.5f;
    clover_float phase_offset  = 0.25f;

    osc.freq(freq);
    osc.phase(initial_phase);
    osc.phase_offset(phase_offset);

    EXPECT_FLOAT_EQ(osc.freq(), freq);
    EXPECT_FLOAT_EQ(osc.phase(), initial_phase + phase_offset);
    EXPECT_FLOAT_EQ(osc.phase_offset(), phase_offset);
}

TEST(dsp_oscillator, proxies_to_phase_tracker) {
    oscillator osc(48000.f);

    osc.freq(100.f);
    osc.phase(0.5f);
    osc.phase_offset(0.25f);

    EXPECT_FLOAT_EQ(osc.freq(), osc.phase_tracker.freq());
    EXPECT_FLOAT_EQ(osc.phase(), osc.phase_tracker.phase());
    EXPECT_FLOAT_EQ(osc.phase_offset(), osc.phase_tracker.phase_offset());
    EXPECT_FLOAT_EQ(osc.period(), osc.phase_tracker.period());
}
