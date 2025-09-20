// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <format>
#include <stdexcept>

#include "clover/circular_buffer.hpp"

namespace clover::dsp {

circular_buffer::circular_buffer(std::shared_ptr<std::vector<float>>& underlying)
    : m_underlying(underlying), m_length(underlying->size()), m_current(underlying->size() - 1) {
}

circular_buffer::circular_buffer(size_t length)
    : m_underlying(std::make_shared<std::vector<float>>(length, 0.0f)),
      m_length(length),
      m_current(length - 1) {
}

void circular_buffer::tick(float x) {
    if (++m_current == m_length) {
        m_current = 0;
    }
    (*m_underlying)[m_current] = x;
}

size_t circular_buffer::length() {
    return m_length;
}

float& circular_buffer::operator[](size_t idx) {
    if (idx >= m_length) {
        throw std::out_of_range(std::format("out of range: circular_buffer::operator[{}]", idx));
    }

    size_t offset = (m_current + m_length - idx) % m_length;
    return (*m_underlying)[offset];
}

circular_buffer_2::circular_buffer_2(std::shared_ptr<std::vector<float>>& underlying)
    : m_underlying(underlying), m_length(underlying->size() / 2), m_current((underlying->size() / 2) - 2) {
}

circular_buffer_2::circular_buffer_2(size_t length)
    : m_underlying(std::make_shared<std::vector<float>>(length * 2, 0.0f)),
      m_length(length),
      m_current((length * 2) - 2) {
}

void circular_buffer_2::tick(float L, float R) {
    m_current += 2;
    if (m_current >= 2 * m_length) {
        m_current = 0;
    }

    (*m_underlying)[m_current]     = L;
    (*m_underlying)[m_current + 1] = R;
}

size_t circular_buffer_2::length() {
    return m_length;
}

std::pair<float&, float&> circular_buffer_2::operator[](size_t idx) {
    if (idx < 0 || idx >= m_length)
        throw std::out_of_range(std::format("out of range: circular_buffer_2::operator[{}]", idx));

    size_t interleaved_idx = 2 * idx;

    if (m_current < interleaved_idx)
        return {
                (*m_underlying)[m_current + (2 * m_length) - interleaved_idx],     //
                (*m_underlying)[m_current + (2 * m_length) - interleaved_idx + 1]  //
        };
    return {
            (*m_underlying)[m_current - interleaved_idx],     //
            (*m_underlying)[m_current - interleaved_idx + 1]  //
    };
}

}  // namespace clover::dsp
