// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>

#include "clover/dsp/env_adsr.hpp"

namespace clover::dsp {

env_adsr::env_adsr() = default;

void env_adsr::set(float a, float d, float s, float r) {
    attack(a);
    decay(d);
    sustain(s);
    release(r);
}

void env_adsr::set(float a, float a_gain, float d, float s, float r) {
    attack(a);
    attack_gain(a_gain);
    decay(d);
    sustain(s);
    release(r);
}

float env_adsr::attack() {
    return m_attack_samples;
}

float env_adsr::attack_gain() {
    return m_attack_gain;
}

float env_adsr::decay() {
    return m_decay_samples;
}

float env_adsr::sustain() {
    return m_sustain;
}

float env_adsr::release() {
    return m_release_samples;
}

void env_adsr::attack(float duration) {
    m_attack_samples = std::max(duration, 1.f);
    if (m_state == state::attack) {
        m_env.m_target_step = duration;
    }
}

void env_adsr::attack_gain(float a_gain) {
    m_attack_gain = a_gain;
    if (m_state == state::attack) {
        m_env.set(m_attack_gain);
    }
}

void env_adsr::decay(float duration) {
    m_decay_samples = std::max(duration, 1.f);
    if (m_state == state::decay) {
        m_env.m_target_step = duration;
    }
}

void env_adsr::sustain(float value) {
    m_sustain = value;

    if (m_state == state::decay || m_state == state::sustain) {
        m_env.m_to = value;
    }
}

void env_adsr::release(float duration) {
    m_release_samples = std::max(duration, 1.f);
    if (m_state == state::release) {
        m_env.m_target_step = duration;
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
        m_env.set(0, m_release_samples);

        // churn one tick to begin release on next sample
        m_env.tick();
    }
}

bool env_adsr::is_active() {
    return m_state >= state::attack;
}

float env_adsr::tick() {
    float signal = m_env.tick();

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
