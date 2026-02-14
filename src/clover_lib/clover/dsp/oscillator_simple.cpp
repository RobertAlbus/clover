// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/oscillator_simple.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

oscillator_simple::oscillator_simple(float fs) : waveform(wave_sine), m_fs(fs) {
}

auto oscillator_simple::tick() -> float {
    m_current_phase += m_phase_increment;
    if (m_current_phase >= num::pi_x2 || m_current_phase < 0.f) {
        m_current_phase = std::fmod(m_current_phase, num::pi_x2);
        if (m_current_phase < 0.f)
            m_current_phase += num::pi_x2;
    }

    if (waveform)
        return waveform(m_current_phase);

    return 0;
}

auto oscillator_simple::freq(float freq_hz) -> void {
    m_phase_increment = freq_hz != 0 ? (freq_hz / m_fs) * num::pi_x2 : 0;
}

auto oscillator_simple::freq() const -> float {
    return (m_phase_increment / num::pi_x2) * m_fs;
}

auto oscillator_simple::period(float num_samples) -> void {
    m_phase_increment = num_samples != 0 ? num::pi_x2 / num_samples : 0;
}

auto oscillator_simple::period() const -> float {
    return m_phase_increment != 0 ? num::pi_x2 / m_phase_increment : 0;
}

auto oscillator_simple::phase(float p) -> void {
    if (p >= num::pi_x2)
        p = std::fmod(p, num::pi_x2);
    if (p < 0.f)
        p = std::fmod(std::fmod(p, num::pi_x2) + num::pi_x2, num::pi_x2);

    m_current_phase = p + m_phase_offset;
    if (m_current_phase >= num::pi_x2)
        m_current_phase -= num::pi_x2;
    if (m_current_phase < 0.f)
        m_current_phase += num::pi_x2;
}

auto oscillator_simple::phase() const -> float {
    return m_current_phase;
}

auto oscillator_simple::phase_offset(float offset) -> void {
    if (offset >= num::pi_x2)
        offset = std::fmod(offset, num::pi_x2);
    if (offset < 0.f)
        offset = std::fmod(std::fmod(offset, num::pi_x2) + num::pi_x2, num::pi_x2);

    m_current_phase -= m_phase_offset;
    m_current_phase += offset;
    m_phase_offset = offset;

    if (m_current_phase < 0.f)
        m_current_phase += num::pi_x2;
    else if (m_current_phase >= num::pi_x2)
        m_current_phase -= num::pi_x2;
}

auto oscillator_simple::phase_offset() const -> float {
    return m_phase_offset;
}

}  // namespace clover::dsp
