// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <ranges>

#include <gtest/gtest.h>

#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/float.hpp"
#include "clover/math.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_filter, iir_coeffs_initialize_blank) {
    iir_coeffs coeffs;

    EXPECT_FLOAT_EQ(coeffs.b0, 0.f);
    EXPECT_FLOAT_EQ(coeffs.b1, 0.f);
    EXPECT_FLOAT_EQ(coeffs.b2, 0.f);
    EXPECT_FLOAT_EQ(coeffs.b1, 0.f);
    EXPECT_FLOAT_EQ(coeffs.b2, 0.f);
}

TEST(dsp_filter, initializes_blank) {
    filter filt{};

    EXPECT_FLOAT_EQ(filt.m_coeffs.b0, 0.f);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b1, 0.f);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b2, 0.f);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b1, 0.f);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b2, 0.f);
}

TEST(dsp_filter, initializes_with_coeffs_by_val) {
    filter filt{lpf(48000, 440, 1)};

    iir_coeffs compare = lpf(48000, 440, 1);

    EXPECT_FLOAT_EQ(filt.m_coeffs.b0, compare.b0);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b1, compare.b1);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b2, compare.b2);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b1, compare.b1);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b2, compare.b2);
}

TEST(dsp_filter, initializes_with_coeffs_by_ref) {
    iir_coeffs coeffs = lpf(48000, 440, 1);

    filter filt{coeffs};

    EXPECT_FLOAT_EQ(filt.m_coeffs.b0, coeffs.b0);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b1, coeffs.b1);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b2, coeffs.b2);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b1, coeffs.b1);
    EXPECT_FLOAT_EQ(filt.m_coeffs.b2, coeffs.b2);
}

TEST(dsp_filter, initializes_with_empty_state) {
    iir_coeffs coeffs = lpf(48000, 440, 1);

    std::vector<filter> filters{
            filter{},
            filter{coeffs},
            filter{lpf(48000, 440, 1)},
    };

    for (auto& filt : filters) {
        for (auto x : filt.m_inputs)
            EXPECT_FLOAT_EQ(x, 0.f);
        for (auto x : filt.m_outputs)
            EXPECT_FLOAT_EQ(x, 0.f);
    }
}

TEST(dsp_filter, filters_signal) {
    clover_float attenuation = db_to_linear(-50);

    clover_float fs     = 48000;
    clover_float cutoff = 500;
    filter filt{lpf(fs, cutoff, 1)};

    clover_float pitch = 10800;
    oscillator osc(fs);
    osc.freq(pitch);

    clover_float fade_duration = 5;
    for (auto n : std::views::iota(0, (int)fs)) {
        clover_float fade   = ((clover_float)n < fade_duration) ? (((clover_float)n) / fade_duration) : 1;
        clover_float sample = osc.tick() * fade;

        sample = filt.tick(sample);

        EXPECT_LE(sample, attenuation);
        EXPECT_GE(sample, -attenuation);
    }
}