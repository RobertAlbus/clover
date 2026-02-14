#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>

namespace clover::dsp {

struct oscillator_simple {
    oscillator_simple(float fs);
    oscillator_simple() = delete;

    std::function<float(float)> waveform;

    float tick();

    void freq(float freq_hz);
    float freq() const;

    void period(float num_samples);
    float period() const;

    void phase(float phase);
    float phase() const;

    void phase_offset(float offset);
    float phase_offset() const;

   private:
    float m_fs;
    float m_current_phase   = 0;
    float m_phase_increment = 0;
    float m_phase_offset    = 0;
};

}  // namespace clover::dsp
