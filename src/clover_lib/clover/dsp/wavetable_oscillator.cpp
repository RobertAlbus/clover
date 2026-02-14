// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/wavetable.hpp"
#include "clover/dsp/wavetable_oscillator.hpp"

namespace clover::dsp {

wavetable_oscillator::wavetable_oscillator(float fs) : m_table(&wavetable::sine), m_fs(fs) {
}

wavetable_oscillator::wavetable_oscillator(float fs, const std::vector<float>& table)
    : m_table(&table), m_fs(fs) {
}

auto wavetable_oscillator::tick() -> float {
    m_current_phase += m_phase_increment;
    if (m_current_phase >= 1.f || m_current_phase < 0.f) {
        m_current_phase = std::fmod(m_current_phase, 1.f);
        if (m_current_phase < 0.f)
            m_current_phase += 1.f;
    }

    auto table_size_f = static_cast<float>(m_table->size());
    float pos         = m_current_phase * table_size_f;
    auto idx0         = static_cast<size_t>(pos);
    if (idx0 >= m_table->size())
        idx0 = m_table->size() - 1;
    size_t idx1 = (idx0 + 1) % m_table->size();
    float frac  = pos - static_cast<float>(idx0);

    return (*m_table)[idx0] + frac * ((*m_table)[idx1] - (*m_table)[idx0]);
}

auto wavetable_oscillator::freq(float freq_hz) -> void {
    m_phase_increment = freq_hz != 0 ? freq_hz / m_fs : 0;
}

auto wavetable_oscillator::freq() const -> float {
    return m_phase_increment * m_fs;
}

auto wavetable_oscillator::period(float num_samples) -> void {
    m_phase_increment = num_samples != 0 ? 1.f / num_samples : 0;
}

auto wavetable_oscillator::period() const -> float {
    return m_phase_increment != 0 ? 1.f / m_phase_increment : 0;
}

auto wavetable_oscillator::phase(float p) -> void {
    if (p >= 1.f)
        p = std::fmod(p, 1.f);
    if (p < 0.f)
        p = std::fmod(std::fmod(p, 1.f) + 1.f, 1.f);

    m_current_phase = p + m_phase_offset;
    if (m_current_phase >= 1.f)
        m_current_phase -= 1.f;
    if (m_current_phase < 0.f)
        m_current_phase += 1.f;
}

auto wavetable_oscillator::phase() const -> float {
    return m_current_phase;
}

auto wavetable_oscillator::phase_offset(float offset) -> void {
    if (offset >= 1.f)
        offset = std::fmod(offset, 1.f);
    if (offset < 0.f)
        offset = std::fmod(std::fmod(offset, 1.f) + 1.f, 1.f);

    m_current_phase -= m_phase_offset;
    m_current_phase += offset;
    m_phase_offset = offset;

    if (m_current_phase < 0.f)
        m_current_phase += 1.f;
    else if (m_current_phase >= 1.f)
        m_current_phase -= 1.f;
}

auto wavetable_oscillator::phase_offset() const -> float {
    return m_phase_offset;
}

auto wavetable_oscillator::waveform(const std::vector<float>& table) -> void {
    m_table = &table;
}

}  // namespace clover::dsp
