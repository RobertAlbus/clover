#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "clover/float.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

struct phase_increment_tracker {
    clover_float m_current_phase   = 0;  // in samples
    clover_float m_phase_increment = 0;
    clover_float m_max_phase       = 0;  // in samples
    clover_float m_phase_offset    = 0;  // relative to domain
    clover_float m_domain          = num::pi_x2;
    clover_float m_fs              = 0;

    static auto for_freq(clover_float fs, clover_float freq_hz, clover_float domain = num::pi_x2) noexcept
            -> phase_increment_tracker;

    static auto for_period(
            clover_float fs,
            clover_float num_samples,
            clover_float domain = num::pi_x2) noexcept -> phase_increment_tracker;

    void tick() noexcept;

    void freq(clover_float freq_hz) noexcept;
    clover_float freq() noexcept;

    void period(clover_float num_samples) noexcept;
    clover_float period() noexcept;  // in samples

    void phase(clover_float phase) noexcept;  // relative to domain
    clover_float phase() noexcept;            // relative to domain

    void phase_offset(clover_float offset) noexcept;  // relative to domain
    clover_float phase_offset() noexcept;             // relative to domain
};

}  // namespace clover::dsp
