// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "clover/dsp/phase_increment_tracker.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

auto phase_increment_tracker::for_freq(clover_float fs, clover_float freq_hz, clover_float domain) noexcept
        -> phase_increment_tracker {
    auto pit = phase_increment_tracker{
            .m_current_phase   = 0,
            .m_phase_increment = 0,
            .m_max_phase       = 0,
            .m_phase_offset    = 0,
            .m_domain          = domain,
            .m_fs              = fs};

    pit.freq(freq_hz);
    pit.phase(0);

    return pit;
}

auto phase_increment_tracker::for_period(
        clover_float fs, clover_float num_samples, clover_float domain) noexcept -> phase_increment_tracker {
    auto pit = phase_increment_tracker{
            .m_current_phase   = 0,
            .m_phase_increment = 0,
            .m_max_phase       = 0,
            .m_phase_offset    = 0,
            .m_domain          = domain,
            .m_fs              = fs};

    pit.period(num_samples);
    pit.phase(0);
    pit.phase_offset(0);

    return pit;
}

auto phase_increment_tracker::tick() noexcept -> void {
    m_current_phase += m_phase_increment;
    if (m_current_phase >= m_max_phase) {
        m_current_phase -= m_max_phase;
    }
}

auto phase_increment_tracker::freq(clover_float freq_hz) noexcept -> void {
    freq_hz = std::clamp(freq_hz, clover_float{0}, m_fs / 2.f);
    freq_hz != 0 ? period(m_fs / freq_hz) : period(0);
}

auto phase_increment_tracker::freq() noexcept -> clover_float {
    return m_phase_increment != 0 ? m_fs / m_max_phase : 0;
}

auto phase_increment_tracker::period(clover_float num_samples) noexcept -> void {
    m_max_phase       = std::max(num_samples, clover_float{1});
    m_phase_increment = num_samples != 0 ? 1 : 0;
}

auto phase_increment_tracker::period() noexcept -> clover_float {
    return m_phase_increment != 0 ? m_max_phase : 0;
}

auto phase_increment_tracker::phase(clover_float phase) noexcept -> void {
    if (phase >= m_domain)
        phase = std::fmod(phase, m_domain);
    if (phase < 0) {
        phase = std::fmod(std::fmod(phase, m_domain) + m_domain, m_domain);
    }
    m_current_phase = ((phase / m_domain) * m_max_phase) + ((m_phase_offset / m_domain) * m_max_phase);
}

auto phase_increment_tracker::phase() noexcept -> clover_float {
    return std::lerp(clover_float{0}, m_domain, m_current_phase / m_max_phase);
}

auto phase_increment_tracker::phase_offset(clover_float offset) noexcept -> void {
    if (offset >= m_domain)
        offset = std::fmod(offset, m_domain);
    if (offset < 0) {
        offset = std::fmod(std::fmod(offset, m_domain) + m_domain, m_domain);
    }

    clover_float previous_offset = ((m_phase_offset / m_domain) * m_max_phase);
    clover_float new_offset      = ((offset / m_domain) * m_max_phase);
    m_current_phase -= previous_offset;
    m_current_phase += new_offset;

    m_phase_offset = offset;

    if (m_current_phase < 0) {
        m_current_phase += m_max_phase;
    } else if (m_current_phase >= m_max_phase) {
        m_current_phase -= m_max_phase;
    }
}

auto phase_increment_tracker::phase_offset() noexcept -> clover_float {
    return m_phase_offset;
}

}  // namespace clover::dsp