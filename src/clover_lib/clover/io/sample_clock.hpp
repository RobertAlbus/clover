#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstdint>

namespace clover::io {

class sample_clock {
   public:
    sample_clock();
    int_fast64_t current_sample() const;
    void tick();

    int_fast64_t m_current_sample;
};

}  // namespace clover::io
