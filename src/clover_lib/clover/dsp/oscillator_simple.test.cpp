// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include <gtest/gtest.h>

#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/oscillator_simple.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/num.hpp"

using namespace clover;
using namespace dsp;

static float circular_distance(float a, float b, float period) {
    float d = std::fmod(std::abs(a - b), period);
    return std::min(d, period - d);
}

static bool near_discontinuity(float phase_a, float phase_b, float disc_point, float tol) {
    return circular_distance(phase_a, disc_point, num::pi_x2) < tol ||
           circular_distance(phase_b, disc_point, num::pi_x2) < tol;
}

TEST(dsp_oscillator_simple, initializes_blank) {
    float fs = 48000.f;

    oscillator_simple osc(fs);
    EXPECT_FLOAT_EQ(osc.freq(), 0.f);
    EXPECT_FLOAT_EQ(osc.phase(), 0.f);
    EXPECT_FLOAT_EQ(osc.phase_offset(), 0.f);
    EXPECT_FLOAT_EQ(osc.period(), 0.f);
}

TEST(dsp_oscillator_simple, sets_correctly) {
    float fs = 48000.f;

    oscillator_simple osc(fs);

    float freq          = 100.f;
    float initial_phase = 0.5f;
    float phase_offset  = 0.25f;

    osc.freq(freq);
    osc.phase(initial_phase);
    osc.phase_offset(phase_offset);

    EXPECT_FLOAT_EQ(osc.freq(), freq);
    EXPECT_NEAR(osc.phase(), initial_phase + phase_offset, 1e-6);
    EXPECT_FLOAT_EQ(osc.phase_offset(), phase_offset);
}

TEST(dsp_oscillator_simple, parity_sine) {
    float fs   = 48000.f;
    float freq = 384.f;

    oscillator_simple simple(fs);
    simple.waveform = wave_sine;
    oscillator osc(fs);
    osc.waveform = wave_sine;

    simple.freq(freq);
    osc.freq(freq);

    for (int s = 0; s < 1000; ++s) {
        float simple_out = simple.tick();
        float osc_out    = osc.tick();
        EXPECT_NEAR(simple_out, osc_out, 1e-4) << "sample " << s;
    }
}

TEST(dsp_oscillator_simple, parity_square) {
    float fs   = 48000.f;
    float freq = 384.f;

    oscillator_simple simple(fs);
    simple.waveform = wave_square;
    oscillator osc(fs);
    osc.waveform = wave_square;

    simple.freq(freq);
    osc.freq(freq);

    float boundary_tol = freq / fs * num::pi_x2 * 2;
    for (int s = 0; s < 1000; ++s) {
        float simple_out = simple.tick();
        float osc_out    = osc.tick();
        if (near_discontinuity(simple.phase(), osc.phase(), num::pi, boundary_tol) ||
            near_discontinuity(simple.phase(), osc.phase(), 0.f, boundary_tol))
            continue;
        EXPECT_NEAR(simple_out, osc_out, 1e-3) << "sample " << s;
    }
}

TEST(dsp_oscillator_simple, parity_saw) {
    float fs   = 48000.f;
    float freq = 384.f;

    oscillator_simple simple(fs);
    simple.waveform = wave_saw;
    oscillator osc(fs);
    osc.waveform = wave_saw;

    simple.freq(freq);
    osc.freq(freq);

    float boundary_tol = freq / fs * num::pi_x2 * 2;
    for (int s = 0; s < 1000; ++s) {
        float simple_out = simple.tick();
        float osc_out    = osc.tick();
        if (near_discontinuity(simple.phase(), osc.phase(), 0.f, boundary_tol))
            continue;
        EXPECT_NEAR(simple_out, osc_out, 1e-4) << "sample " << s;
    }
}

TEST(dsp_oscillator_simple, parity_tri) {
    float fs   = 48000.f;
    float freq = 384.f;

    oscillator_simple simple(fs);
    simple.waveform = wave_tri;
    oscillator osc(fs);
    osc.waveform = wave_tri;

    simple.freq(freq);
    osc.freq(freq);

    for (int s = 0; s < 1000; ++s) {
        float simple_out = simple.tick();
        float osc_out    = osc.tick();
        EXPECT_NEAR(simple_out, osc_out, 1e-4) << "sample " << s;
    }
}

TEST(dsp_oscillator_simple, phase_wrapping_forward) {
    float fs   = 48000.f;
    float freq = 384.f;

    oscillator_simple osc(fs);
    osc.freq(freq);

    int period_samples = static_cast<int>(fs / freq);
    for (int s = 0; s < period_samples; ++s) {
        osc.tick();
    }
    float p              = osc.phase();
    float dist_from_zero = std::min(p, num::pi_x2 - p);
    EXPECT_NEAR(dist_from_zero, 0.f, 1e-3);
}

TEST(dsp_oscillator_simple, phase_wrapping_backward) {
    float fs   = 48000.f;
    float freq = -384.f;

    oscillator_simple osc(fs);
    osc.freq(freq);

    int period_samples = static_cast<int>(fs / std::abs(freq));
    for (int s = 0; s < period_samples; ++s) {
        osc.tick();
    }
    float p              = osc.phase();
    float dist_from_zero = std::min(p, num::pi_x2 - p);
    EXPECT_NEAR(dist_from_zero, 0.f, 1e-3);
}

TEST(dsp_oscillator_simple, negative_freq) {
    float fs   = 48000.f;
    float freq = -384.f;

    oscillator_simple osc(fs);
    osc.freq(freq);

    EXPECT_FLOAT_EQ(osc.freq(), freq);

    osc.tick();
    float expected_phase = num::pi_x2 - (std::abs(freq) / fs) * num::pi_x2;
    EXPECT_NEAR(osc.phase(), expected_phase, 1e-4);
}

TEST(dsp_oscillator_simple, freq_zero_freezes_phase) {
    float fs = 48000.f;

    oscillator_simple osc(fs);
    osc.freq(384.f);
    osc.phase(num::pi);
    float phase_before = osc.phase();

    osc.freq(0.f);
    for (int i = 0; i < 100; ++i) {
        osc.tick();
    }
    EXPECT_FLOAT_EQ(osc.phase(), phase_before);
}

TEST(dsp_oscillator_simple, phase_offset) {
    float fs = 48000.f;

    oscillator_simple osc(fs);
    osc.freq(1.f);

    osc.phase_offset(num::pi / 2);
    EXPECT_FLOAT_EQ(osc.phase_offset(), num::pi / 2);
    EXPECT_NEAR(osc.phase(), num::pi / 2, 1e-6);

    osc.phase(0.5f);
    EXPECT_NEAR(osc.phase(), 0.5f + num::pi / 2, 1e-6);
}

TEST(dsp_oscillator_simple, above_nyquist) {
    float fs   = 48000.f;
    float freq = 64000.f;

    oscillator_simple osc(fs);
    osc.freq(freq);

    for (int i = 0; i < 100; ++i) {
        float out = osc.tick();
        EXPECT_GE(out, -1.1f);
        EXPECT_LE(out, 1.1f);
    }
}

TEST(dsp_oscillator_simple, waveform_change) {
    float fs = 48000.f;

    oscillator_simple osc(fs);
    osc.freq(384.f);

    osc.tick();
    float sine_out = osc.tick();

    osc.phase(0.f);
    osc.waveform = wave_saw;

    osc.tick();
    float saw_out = osc.tick();

    EXPECT_NE(sine_out, saw_out);
}
