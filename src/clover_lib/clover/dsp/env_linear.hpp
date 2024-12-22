#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/float.hpp"

namespace clover::dsp {

struct env_linear {
    clover_float m_current_step = 0;
    clover_float m_target_step  = 0;
    clover_float m_from         = 0;
    clover_float m_to           = 0;

    // new envelope from a to b over n samples
    void set(clover_float a, clover_float b, clover_float n);

    // new envelope from current value to b over n samples
    void set(clover_float b, clover_float n);

    // update envelope b without restarting
    void set(clover_float b);

    clover_float tick();
};

}  // namespace clover::dsp