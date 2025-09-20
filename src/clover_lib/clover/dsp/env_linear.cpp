// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_linear.hpp"
#include <cmath>

namespace clover::dsp {

void env_linear::set(float a, float b, float n) {
    m_current_step = 0;
    m_target_step  = std::max(n, 0.f);
    m_from         = a;
    m_to           = b;
}

void env_linear::set(float b, float n) {
    m_from = std::lerp(m_from, m_to, m_current_step / m_target_step);
    m_to   = b;

    m_current_step = 0;
    m_target_step  = std::max(n, 0.f);
}

void env_linear::set(float b) {
    m_from         = std::lerp(m_from, m_to, m_current_step / m_target_step);
    m_target_step  = m_target_step - m_current_step;
    m_current_step = 0;

    m_to = b;
}

float env_linear::tick() {
    if (m_current_step < m_target_step) {
        return std::lerp(m_from, m_to, m_current_step++ / m_target_step);
    }
    return m_to;
}

float env_linear::current_value() {
    if (m_current_step < m_target_step) {
        return std::lerp(m_from, m_to, m_current_step / m_target_step);
    }
    return m_to;
}

}  // namespace clover::dsp