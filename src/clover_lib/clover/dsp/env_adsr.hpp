#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_linear.hpp"

namespace clover::dsp {

struct env_adsr {
    enum struct state {
        init,
        complete,
        attack,
        decay,
        sustain,
        release,
    };

    float m_attack_samples  = 0;
    float m_attack_gain     = 1;
    float m_sustain         = 0;
    float m_decay_samples   = 0;
    float m_release_samples = 0;
    float m_elapsed_samples = 1;

    state m_state = state::init;
    env_linear m_env;

    env_adsr();

    void set(float a, float d, float s, float r);
    void set(float a, float a_gain, float d, float s, float r);
    float attack();
    float attack_gain();
    float decay();
    float sustain();
    float release();

    void attack(float duration);
    void attack_gain(float duration);
    void decay(float duration);
    void sustain(float value);
    void release(float duration);

    void key_on();
    void key_off();

    bool is_active();

    float tick();
};

}  // namespace clover::dsp