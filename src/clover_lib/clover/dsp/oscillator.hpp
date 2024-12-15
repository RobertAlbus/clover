#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <memory>

#include "clover/dsp/phase_increment_tracker.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

struct oscillator {
    oscillator(clover_float fs);
    oscillator() = delete;

    phase_increment_tracker phase_tracker;
    std::unique_ptr<std::function<clover_float(clover_float)>> waveform;

    clover_float tick();

    void freq(clover_float freq_hz);
    clover_float freq();

    void note(clover_float midi_note);
    clover_float note();

    void period(clover_float num_samples);
    clover_float period();  // in samples

    void phase(clover_float phase);  // relative to domain
    clover_float phase();            // relative to domain

    void phase_offset(clover_float offset);  // relative to domain
    clover_float phase_offset();             // relative to domain
};

}  // namespace clover::dsp
