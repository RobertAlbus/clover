#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

namespace clover::dsp {

struct wavetable_oscillator {
    wavetable_oscillator(float fs);
    wavetable_oscillator(float fs, const std::vector<float>& table);
    wavetable_oscillator() = delete;

    float tick();

    void freq(float freq_hz);
    float freq() const;

    void period(float num_samples);
    float period() const;

    void phase(float phase);
    float phase() const;

    void phase_offset(float offset);
    float phase_offset() const;

    void waveform(const std::vector<float>& table);

   private:
    const std::vector<float>* m_table;
    float m_fs;
    float m_current_phase   = 0;
    float m_phase_increment = 0;
    float m_phase_offset    = 0;
};

}  // namespace clover::dsp
