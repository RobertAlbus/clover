// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/dsp/wavetable.hpp"
#include "clover/dsp/wavetable_oscillator.hpp"

using namespace clover;
using namespace dsp;

TEST(dsp_wavetable_oscillator, initializes_blank) {
    float fs = 48000.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    EXPECT_FLOAT_EQ(wt.freq(), 0.f);
    EXPECT_FLOAT_EQ(wt.phase(), 0.f);
    EXPECT_FLOAT_EQ(wt.phase_offset(), 0.f);
    EXPECT_FLOAT_EQ(wt.period(), 0.f);
}

TEST(dsp_wavetable_oscillator, sets_correctly) {
    float fs = 48000.f;

    wavetable_oscillator wt(fs, wavetable::sine);

    float freq          = 100.f;
    float initial_phase = 0.3f;
    float phase_offset  = 0.1f;

    wt.freq(freq);
    wt.phase(initial_phase);
    wt.phase_offset(phase_offset);

    EXPECT_FLOAT_EQ(wt.freq(), freq);
    EXPECT_NEAR(wt.phase(), initial_phase + phase_offset, 1e-6);
    EXPECT_FLOAT_EQ(wt.phase_offset(), phase_offset);
}

TEST(dsp_wavetable_oscillator, parity_sine) {
    float fs   = 48000.f;
    float freq = 384.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    oscillator osc(fs);
    osc.waveform = wave_sine;

    wt.freq(freq);
    osc.freq(freq);

    for (int s = 0; s < 1000; ++s) {
        float wt_out  = wt.tick();
        float osc_out = osc.tick();
        EXPECT_NEAR(wt_out, osc_out, 1e-4) << "sample " << s;
    }
}

TEST(dsp_wavetable_oscillator, parity_square) {
    float fs   = 48000.f;
    float freq = 384.f;

    wavetable_oscillator wt(fs, wavetable::square);
    oscillator osc(fs);
    osc.waveform = wave_square;

    wt.freq(freq);
    osc.freq(freq);

    for (int s = 0; s < 1000; ++s) {
        float wt_out  = wt.tick();
        float osc_out = osc.tick();
        EXPECT_NEAR(wt_out, osc_out, 1e-3) << "sample " << s;
    }
}

TEST(dsp_wavetable_oscillator, parity_saw) {
    float fs   = 48000.f;
    float freq = 384.f;

    wavetable_oscillator wt(fs, wavetable::saw);
    oscillator osc(fs);
    osc.waveform = wave_saw;

    wt.freq(freq);
    osc.freq(freq);

    for (int s = 0; s < 1000; ++s) {
        float wt_out  = wt.tick();
        float osc_out = osc.tick();
        EXPECT_NEAR(wt_out, osc_out, 1e-4) << "sample " << s;
    }
}

TEST(dsp_wavetable_oscillator, parity_tri) {
    float fs   = 48000.f;
    float freq = 384.f;

    wavetable_oscillator wt(fs, wavetable::tri);
    oscillator osc(fs);
    osc.waveform = wave_tri;

    wt.freq(freq);
    osc.freq(freq);

    for (int s = 0; s < 1000; ++s) {
        float wt_out  = wt.tick();
        float osc_out = osc.tick();
        EXPECT_NEAR(wt_out, osc_out, 1e-4) << "sample " << s;
    }
}

TEST(dsp_wavetable_oscillator, custom_table) {
    float fs = 48000.f;

    std::vector<float> custom(wavetable::table_size, 0.5f);

    wavetable_oscillator wt(fs, custom);
    wt.freq(100.f);

    for (int s = 0; s < 100; ++s) {
        EXPECT_FLOAT_EQ(wt.tick(), 0.5f);
    }
}

TEST(dsp_wavetable_oscillator, phase_wrapping_forward) {
    float fs   = 48000.f;
    float freq = 384.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    wt.freq(freq);

    int period_samples = static_cast<int>(fs / freq);
    for (int s = 0; s < period_samples; ++s) {
        wt.tick();
    }
    EXPECT_NEAR(wt.phase(), 0.f, 1e-5);
}

TEST(dsp_wavetable_oscillator, phase_wrapping_backward) {
    float fs   = 48000.f;
    float freq = -384.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    wt.freq(freq);

    int period_samples = static_cast<int>(fs / std::abs(freq));
    for (int s = 0; s < period_samples; ++s) {
        wt.tick();
    }
    float p              = wt.phase();
    float dist_from_zero = std::min(p, 1.f - p);
    EXPECT_NEAR(dist_from_zero, 0.f, 1e-5);
}

TEST(dsp_wavetable_oscillator, negative_freq) {
    float fs   = 48000.f;
    float freq = -384.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    wt.freq(freq);

    EXPECT_FLOAT_EQ(wt.freq(), freq);

    wt.tick();
    float expected_phase = 1.f - std::abs(freq) / fs;
    EXPECT_NEAR(wt.phase(), expected_phase, 1e-6);
}

TEST(dsp_wavetable_oscillator, freq_zero_freezes_phase) {
    float fs = 48000.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    wt.freq(384.f);
    wt.phase(0.5f);
    float phase_before = wt.phase();

    wt.freq(0.f);
    for (int i = 0; i < 100; ++i) {
        wt.tick();
    }
    EXPECT_FLOAT_EQ(wt.phase(), phase_before);
}

TEST(dsp_wavetable_oscillator, phase_offset) {
    float fs = 48000.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    wt.freq(1.f);

    wt.phase_offset(0.25f);
    EXPECT_FLOAT_EQ(wt.phase_offset(), 0.25f);
    EXPECT_FLOAT_EQ(wt.phase(), 0.25f);

    wt.phase(0.1f);
    EXPECT_NEAR(wt.phase(), 0.35f, 1e-6);
}

TEST(dsp_wavetable_oscillator, above_nyquist) {
    float fs   = 48000.f;
    float freq = 64000.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    wt.freq(freq);

    for (int i = 0; i < 100; ++i) {
        float out = wt.tick();
        EXPECT_GE(out, -1.1f);
        EXPECT_LE(out, 1.1f);
    }
}

TEST(dsp_wavetable_oscillator, waveform_change) {
    float fs = 48000.f;

    wavetable_oscillator wt(fs, wavetable::sine);
    wt.freq(384.f);

    wt.tick();
    float sine_out = wt.tick();

    wt.phase(0.f);
    wt.waveform(wavetable::saw);

    wt.tick();
    float saw_out = wt.tick();

    EXPECT_NE(sine_out, saw_out);
}
