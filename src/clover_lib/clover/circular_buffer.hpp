#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>

#include "clover/float.hpp"

namespace clover::dsp {

struct circular_buffer {
    std::shared_ptr<std::vector<clover_float>> m_underlying;
    size_t m_current = 0;
    size_t m_length;

    circular_buffer(size_t length);
    circular_buffer(std::shared_ptr<std::vector<clover_float>>& underlying);

    void tick(clover_float x);
    size_t length();
    clover_float& operator[](size_t idx);
};

struct circular_buffer_2 {
    std::shared_ptr<std::vector<clover_float>> m_underlying;
    size_t m_current = 0;
    size_t m_length;

    circular_buffer_2(size_t length);
    circular_buffer_2(std::shared_ptr<std::vector<clover_float>>& underlying);

    void tick(clover_float L, clover_float R);
    size_t length();
    std::pair<clover_float&, clover_float&> operator[](size_t idx);
};

}  // namespace clover::dsp
