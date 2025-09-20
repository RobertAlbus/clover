#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.


namespace clover::dsp {

struct env_linear {
    float m_current_step = 0;
    float m_target_step  = 0;
    float m_from         = 0;
    float m_to           = 0;

    // new envelope from a to b over n samples
    void set(float a, float b, float n);

    // new envelope from current value to b over n samples
    void set(float b, float n);

    // new envelope from current value to b over remaining duration
    void set(float b);

    float tick();
    float current_value();
};

}  // namespace clover::dsp