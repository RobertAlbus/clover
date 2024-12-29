#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <vector>

#include "clover/float.hpp"

namespace clover::dsp {

struct circular_buffer {
    using reverse_iterator = std::vector<clover_float>::reverse_iterator;
    using value_type       = std::vector<clover_float>::value_type;
    using reference        = std::vector<clover_float>::reference;

    reverse_iterator m_rbegin;
    reverse_iterator m_current;
    reverse_iterator m_rend;

    circular_buffer(reverse_iterator rbegin, reverse_iterator rend);
    circular_buffer(std::vector<clover_float>& underlying);

    void tick(value_type x);
    reference operator[](int idx);
};

}  // namespace clover::dsp
