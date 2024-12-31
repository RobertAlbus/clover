// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <format>
#include <stdexcept>

#include "clover/circular_buffer.hpp"

namespace clover::dsp {

circular_buffer::circular_buffer(std::vector<clover_float>& underlying)
    : m_underlying(underlying), m_size(underlying.size()), m_current(underlying.size() - 1) {
}

void circular_buffer::tick(clover_float x) {
    if (++m_current == m_size) {
        m_current = 0;
    }
    m_underlying[m_current] = x;
}

size_t circular_buffer::size() {
    return m_size;
}

clover_float& circular_buffer::operator[](size_t idx) {
    if (idx < 0 || idx >= m_size)
        throw std::out_of_range(std::format("out of range: circular_buffer::operator[{}]", idx));

    if (m_current < idx)
        return m_underlying[m_current + m_size - idx];
    return m_underlying[m_current - idx];
}

}  // namespace clover::dsp
