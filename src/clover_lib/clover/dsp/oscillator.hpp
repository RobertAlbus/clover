#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>

#include "clover/dsp/phase_increment_tracker.hpp"

namespace clover::dsp {

struct oscillator {
    oscillator(float fs);
    oscillator() = delete;

    phase_increment_tracker phase_tracker;
    std::function<float(float)> waveform;

    float tick();

    void freq(float freq_hz);
    float freq();

    void note(float midi_note);
    float note();

    void period(float num_samples);
    float period();  // in samples

    void phase(float phase);  // relative to domain
    float phase();            // relative to domain

    void phase_offset(float offset);  // relative to domain
    float phase_offset();             // relative to domain
};

}  // namespace clover::dsp
