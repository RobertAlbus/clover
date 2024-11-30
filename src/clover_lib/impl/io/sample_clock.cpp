// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "clover/io/sample_clock.hpp"

namespace clover::io {

sample_clock::sample_clock() {
    m_current_sample = 0;
}

int_fast64_t sample_clock::current_sample() const {
    return m_current_sample;
}

void sample_clock::tick() {
    ++m_current_sample;
}

}  // namespace clover::io