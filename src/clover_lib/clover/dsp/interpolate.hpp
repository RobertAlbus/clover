#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>
#include <stdexcept>
#include <vector>

#include "clover/float.hpp"
#include "clover/num.hpp"

namespace clover::dsp {

// assume even size
void generalized_hann_window(int size, clover_float alpha, clover_float* out) {
    constexpr clover_float half = 0.5;
    for (auto n : std::views::iota(0, (size - 1) / 2)) {
        clover_float window_value =
                alpha + (1 - alpha) * half *
                                (1 - std::cos((2 * num::pi * (clover_float)n) / ((clover_float)size - 1)));

        clover_float* i_front = out + n;
        clover_float* i_back  = out + (size - 1 - n);

        *i_front = window_value;
        *i_back  = window_value;
    }
}

void sinc(int size, clover_float interpolate, clover_float* out) {
    for (auto n : std::views::iota(0, (size - 1) / 2)) {
        clover_float sinc_value = 1;
        if (n != 0) {
            clover_float pi_x = num::pi * (float)n;
            sinc_value        = std::sin(pi_x) / pi_x;
        }

        clover_float* i_front = out + n;
        clover_float* i_back  = out + (size - 1 - n);

        *i_front = sinc_value;
        *i_back  = sinc_value;
    }
}

void kernel(int size, const clover_float* window, const clover_float* sinc, clover_float* out) {
    for (auto i : std::views::iota(0, size - 1)) {
        *(out + i) = *(window + i) * *(sinc + i);
    }
}

void windowed_sinc(int size, const clover_float* signal, const clover_float* kernel, clover_float* out) {
    for (auto i : std::views::iota(0, size - 1)) {
        *(out + i) = *(signal + i) * kernel[i];
    }
}

struct interpolate {};

#include <iterator>
#include <ranges>

struct circular_view : std::ranges::view_interface<circular_view> {
    clover_float* m_data;
    size_t m_size;

    circular_view(clover_float* data, size_t size)
        : m_data(data), m_size(size) {
    }

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type        = clover_float*;
        using difference_type   = std::ptrdiff_t;
        using reference         = clover_float*;

        const circular_view* m_view;
        size_t m_from;

        reference operator*() const {
            return m_view->m_data + m_from;
        }

        iterator& operator++() {
            ++m_from;
            if (m_from >= m_view->m_size)
                ++m_from -= m_view->m_size;
            return *this;
        }

        iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            return m_view == other.m_view && m_from == other.m_from;
        }
    };

    iterator begin() const {
        return iterator{this, 0};
    }

    iterator end() const {
        return iterator{this, m_size};
    }
};

}  // namespace clover::dsp