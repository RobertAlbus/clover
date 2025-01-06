#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_linear.hpp"
#include "clover/float.hpp"

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

    clover_float m_attack_samples  = 0;
    clover_float m_attack_gain     = 1;
    clover_float m_sustain         = 0;
    clover_float m_decay_samples   = 0;
    clover_float m_release_samples = 0;
    clover_float m_elapsed_samples = 1;

    state m_state = state::init;
    env_linear m_env;

    env_adsr();

    void set(clover_float a, clover_float d, clover_float s, clover_float r);
    void set(clover_float a, clover_float a_gain, clover_float d, clover_float s, clover_float r);
    clover_float attack();
    clover_float attack_gain();
    clover_float decay();
    clover_float sustain();
    clover_float release();

    void attack(clover_float duration);
    void attack_gain(clover_float duration);
    void decay(clover_float duration);
    void sustain(clover_float value);
    void release(clover_float duration);

    void key_on();
    void key_off();

    bool is_active();

    clover_float tick();
};

}  // namespace clover::dsp