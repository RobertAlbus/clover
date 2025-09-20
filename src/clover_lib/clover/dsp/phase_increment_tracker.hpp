#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/num.hpp"

namespace clover::dsp {

struct phase_increment_tracker {
    float m_current_phase   = 0;  // in samples
    float m_phase_increment = 0;
    float m_max_phase       = 0;  // in samples
    float m_phase_offset    = 0;  // relative to domain
    float m_domain          = num::pi_x2;
    float m_fs              = 0;

    static auto for_freq(float fs, float freq_hz, float domain = num::pi_x2) noexcept
            -> phase_increment_tracker;

    static auto for_period(float fs, float num_samples, float domain = num::pi_x2) noexcept
            -> phase_increment_tracker;

    void tick() noexcept;

    void freq(float freq_hz) noexcept;
    float freq() noexcept;

    void period(float num_samples) noexcept;
    float period() noexcept;  // in samples

    void phase(float phase) noexcept;  // relative to domain
    float phase() noexcept;            // relative to domain

    void phase_offset(float offset) noexcept;  // relative to domain
    float phase_offset() noexcept;             // relative to domain
};

}  // namespace clover::dsp
