#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstddef>
#include <format>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace clover::dsp {

// General template for circular buffer
template <typename T>
class circular_buffer_t {
   public:
    std::shared_ptr<std::vector<T>> m_underlying;
    size_t m_current = 0;
    size_t m_length;

    explicit circular_buffer_t(size_t length)
        : m_underlying(std::make_shared<std::vector<T>>(length, T{})),
          m_length(length),
          m_current(length - 1) {
    }

    explicit circular_buffer_t(std::shared_ptr<std::vector<T>>& underlying)
        : m_underlying(underlying), m_length(underlying->size()), m_current(underlying->size() - 1) {
    }

    void tick(const T& x) {
        if (++m_current == m_length) {
            m_current = 0;
        }
        (*m_underlying)[m_current] = x;
    }

    size_t length() const {
        return m_length;
    }

    T& operator[](size_t idx) {
        if (idx >= m_length) {
            throw std::out_of_range(std::format("out of range: circular_buffer_t::operator[{}]", idx));
        }

        size_t offset = (m_current + m_length - idx) % m_length;
        return (*m_underlying)[offset];
    }

    const T& operator[](size_t idx) const {
        if (idx >= m_length) {
            throw std::out_of_range(std::format("out of range: circular_buffer_t::operator[{}]", idx));
        }

        size_t offset = (m_current + m_length - idx) % m_length;
        return (*m_underlying)[offset];
    }
};

// Type aliases for backward compatibility
using circular_buffer   = circular_buffer_t<float>;
using circular_buffer_2 = circular_buffer_t<std::pair<float, float>>;

}  // namespace clover::dsp
