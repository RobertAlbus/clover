// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>

#include "clover/dsp/env_adsr.hpp"
#include "clover/float.hpp"

namespace clover::dsp {

env_adsr::env_adsr() = default;

void env_adsr::set(clover_float a, clover_float d, clover_float s, clover_float r) {
    attack(a);
    decay(d);
    sustain(s);
    release(r);
}

void env_adsr::set(clover_float a, clover_float a_gain, clover_float d, clover_float s, clover_float r) {
    attack(a);
    attack_gain(a_gain);
    decay(d);
    sustain(s);
    release(r);
}

clover_float env_adsr::attack() {
    return m_attack_samples;
}

clover_float env_adsr::attack_gain() {
    return m_attack_gain;
}

clover_float env_adsr::decay() {
    return m_decay_samples;
}

clover_float env_adsr::sustain() {
    return m_sustain;
}

clover_float env_adsr::release() {
    return m_release_samples;
}

void env_adsr::attack(clover_float duration) {
    m_attack_samples = std::max(duration, 0.f);

    if (m_state == state::attack) {
        // env_linear treats negative durations as zero.
        m_env.set(m_attack_gain, m_attack_samples - m_env.m_current_step);
    }
}

void env_adsr::attack_gain(clover_float a_gain) {
    m_attack_gain = a_gain;
    if (m_state == state::attack) {
        m_env.set(m_attack_gain, m_attack_samples - m_env.m_current_step);
    }
}

void env_adsr::decay(clover_float duration) {
    if (float_eq(duration, m_decay_samples))
        return;

    m_decay_samples = std::max(duration, 0.f);

    if (m_state == state::decay) {
        // env_linear treats negative durations as zero.
        m_env.set(m_sustain, duration - m_env.m_current_step);
    }
}

void env_adsr::sustain(clover_float value) {
    if (float_eq(value, m_sustain))
        return;

    m_sustain = value;

    if (m_state == state::decay || m_state == state::sustain) {
        m_env.set(value, 0);
    }
}

void env_adsr::release(clover_float duration) {
    if (float_eq(duration, m_release_samples))
        return;

    m_release_samples = std::max(duration, 0.f);

    if (m_state == state::release) {
        // env_linear treats negative durations as zero.
        m_env.set(0, duration - m_env.m_current_step);
    }
}

void env_adsr::key_on() {
    m_state = state::attack;
    m_env.set(0, m_attack_gain, m_attack_samples);
    m_env.tick();
}

void env_adsr::key_off() {
    if (m_state < state::release) {
        m_state = state::release;

        // constant time release, not constant rate.
        // may want to revist this.
        m_env.set(0, m_release_samples);

        // churn one tick to begin release on next sample
        m_env.tick();
    }
}

bool env_adsr::is_active() {
    return m_state >= state::attack;
}

clover_float env_adsr::tick() {
    clover_float signal = m_env.tick();

    switch (m_state) {
        case state::attack: {
            if (m_env.m_current_step >= m_attack_samples) {
                m_env.set(m_sustain, m_decay_samples);
                m_state = state::decay;
            }
            break;
        }
        case state::decay: {
            if (m_env.m_current_step >= m_decay_samples) {
                m_state = state::sustain;
                m_env.set(m_sustain, 0);
            }
            break;
        }
        case state::release: {
            if (m_env.m_current_step >= m_release_samples) {
                m_state = state::complete;
            }
            break;
        }
        default:
            break;
    }

    return signal;
}

}  // namespace clover::dsp