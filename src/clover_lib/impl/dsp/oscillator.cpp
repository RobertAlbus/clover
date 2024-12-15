// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>

#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/phase_increment_tracker.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/float.hpp"
#include "clover/math.hpp"

#include <experimental/simd>

#include <dlfcn.h>

namespace clover::dsp {

oscillator::oscillator(clover_float fs)
    : waveform(std::make_unique<std::function<clover_float(clover_float)>>(&wave_sine)),
      phase_tracker(phase_increment_tracker::for_freq(fs, 0)) {
}

auto oscillator::tick() -> clover_float {
    if (waveform && *waveform) {
        phase_tracker.tick();
        return (*waveform)(phase_tracker.phase());
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

// the following code is adapted from
// https://www.musicdsp.org/en/latest/Synthesis/216-fast-whitenoise-generator.html
// orgiinally by ed.bew@hcrikdlef.dreg

// NOLINTNEXTLINE(bugprone-integer-division)
constexpr clover_float _wave_noise_fScale = 2 / 4294967295.f;

int _wave_noise_x1 = 0x70f4f854;
// NOLINTNEXTLINE(bugprone-narrowing-conversions)
int _wave_noise_x2 = 0xe1e9f0a7;

auto wave_noise(clover_float x) -> clover_float {
    _wave_noise_x1 ^= _wave_noise_x2;
    clover_float signal = static_cast<clover_float>(_wave_noise_x2) * _wave_noise_fScale;
    _wave_noise_x2 += _wave_noise_x1;

    return signal;
}

}  // namespace clover::dsp
