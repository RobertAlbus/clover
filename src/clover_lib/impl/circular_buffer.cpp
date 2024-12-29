// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <iterator>
#include <stdexcept>

#include "clover/circular_buffer.hpp"

namespace clover::dsp {

circular_buffer::circular_buffer(reverse_iterator rbegin, reverse_iterator rend)
    : m_rbegin(rbegin), m_current(rbegin), m_rend(rend) {
}

void circular_buffer::tick(value_type x) {
    m_current[0] = x;
    ++m_current;
    if (m_current == m_rend) {
        m_current = m_rbegin;
    }
}

circular_buffer::reference circular_buffer::operator[](int idx) {
    int size = static_cast<int>(std::distance(m_rbegin, m_rend));
    if (idx < 0 || idx >= size)
        throw std::out_of_range("circular_buffer::operator[]");

    int distance = static_cast<int>(std::distance(m_current, m_rend));
    if (idx >= distance)
        idx -= distance;

    return (m_current + idx)[0];
}

}  // namespace clover::dsp
