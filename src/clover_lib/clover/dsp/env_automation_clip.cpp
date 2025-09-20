// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_automation_clip.hpp"
#include <cmath>
#include <stdexcept>

namespace clover::dsp {

env_automation_clip::env_automation_clip(std::initializer_list<std::pair<const int64_t, float>> map)
    : m_points(map) {
    if (m_points.size() < 2)
        throw std::invalid_argument("env_automation_clip must have least two points");
    if (m_points.cbegin()->first != 0)
        throw std::invalid_argument("env_automation_clip must start at time 0");

    m_current_point = m_points.cbegin();
    m_section_size  = std::next(m_current_point)->first - m_current_point->first;
}
env_automation_clip::env_automation_clip(const std::map<int64_t, float>& map) : m_points(map) {
    if (m_points.size() < 2)
        throw std::invalid_argument("env_automation_clip must have least two points");
    if (m_points.cbegin()->first != 0)
        throw std::invalid_argument("env_automation_clip must start at time 0");

    m_current_point = m_points.cbegin();
    m_section_size  = std::next(m_current_point)->first - m_current_point->first;
}

float env_automation_clip::tick() {
    auto next = std::next(m_current_point);

    if (next == m_points.cend())
        return m_current_point->second;

    if (m_current_sample > m_section_size) {
        m_current_point  = next;
        next             = std::next(m_current_point);
        m_section_size   = next->first - m_current_point->first;
        m_current_sample = 1;
    }

    if (next == m_points.cend())
        return m_current_point->second;

    float percent = static_cast<float>(m_current_sample++) / static_cast<float>(m_section_size);

    return std::lerp(m_current_point->second, next->second, percent);
}

}  // namespace clover::dsp