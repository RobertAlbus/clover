// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>

#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/phase_increment_tracker.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/float.hpp"
#include "clover/math.hpp"

#include <experimental/simd>

#include <dlfcn.h>

namespace clover::dsp {

oscillator::oscillator(clover_float fs)
    : waveform(wave_sine), phase_tracker(phase_increment_tracker::for_freq(fs, 0)) {
}

auto oscillator::tick() -> clover_float {
    if (waveform) {
        phase_tracker.tick();
        return waveform(phase_tracker.phase());
    }
    return 0;
}

auto oscillator::freq(clover_float freq_hz) -> void {
    phase_tracker.freq(freq_hz);
}
auto oscillator::freq() -> clover_float {
    return phase_tracker.freq();
}

auto oscillator::note(clover_float midi_note) -> void {
}
auto oscillator::note() -> clover_float {
    return frequency_to_midi(phase_tracker.freq());
}

auto oscillator::period(clover_float num_samples) -> void {
    phase_tracker.period(num_samples);
}
// in samples
auto oscillator::period() -> clover_float {
    return phase_tracker.period();
}

// relative to domain
auto oscillator::phase(clover_float phase) -> void {
    phase_tracker.phase(phase);
}
// relative to domain
auto oscillator::phase() -> clover_float {
    return phase_tracker.phase();
}

// relative to domain
auto oscillator::phase_offset(clover_float offset) -> void {
    phase_tracker.phase_offset(offset);
}

// relative to domain
auto oscillator::phase_offset() -> clover_float {
    return phase_tracker.phase_offset();
}

auto wave_sine(clover_float x) -> clover_float {
    return std::sin(x);
}
auto wave_square(clover_float x) -> clover_float {
    return x < num::pi ? 1 : 0;
}
auto wave_saw(clover_float x) -> clover_float {
    return std::lerp(clover_float{1}, clover_float{-1}, x / num::pi_x2);
}
auto wave_tri(clover_float x) -> clover_float {
    x += num::pi / 2;
    if (x > num::pi_x2)
        x -= num::pi_x2;
    return -2 * std::abs((x / num::pi) - 1) + 1;
}

int noise_state = 0x67452301;

auto wave_noise(clover_float x) -> clover_float {
    noise_state = (noise_state * 196314165) + 907633515;
    return (float)(noise_state) * 0.0000000004656612873077392578125f;
}

}  // namespace clover::dsp
