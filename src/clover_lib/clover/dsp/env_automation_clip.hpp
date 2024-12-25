#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstdint>
#include <map>

#include "clover/float.hpp"

namespace clover::dsp {

struct env_automation_clip {
    int64_t m_current_sample = 0;
    int64_t m_section_size   = 0;

    std::map<int64_t, clover_float> m_points;
    std::map<int64_t, clover_float>::const_iterator m_current_point;

    env_automation_clip() = delete;
    env_automation_clip(std::initializer_list<std::pair<const int64_t, clover_float>> map);

    env_automation_clip(const std::map<int64_t, clover_float>& existing_map);

    clover_float tick();
};

}  // namespace clover::dsp