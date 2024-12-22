// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_linear.hpp"
#include "clover/float.hpp"
#include <cmath>

namespace clover::dsp {

void env_linear::set(clover_float a, clover_float b, clover_float n) {
    m_current_step = 0;
    m_target_step  = n;
    m_from         = a;
    m_to           = b;
}

void env_linear::set(clover_float b, clover_float n) {
    m_from = std::lerp(m_from, m_to, m_current_step / m_target_step);
    m_to   = b;

    m_current_step = 0;
    m_target_step  = n;
}

void env_linear::set(clover_float b) {
    m_from         = std::lerp(m_from, m_to, m_current_step / m_target_step);
    m_target_step  = m_target_step - m_current_step;
    m_current_step = 0;

    m_to = b;
}

clover_float env_linear::tick() {
    if (m_current_step >= m_target_step)
        return m_to;
    return std::lerp(m_from, m_to, m_current_step++ / m_target_step);
}

}  // namespace clover::dsp