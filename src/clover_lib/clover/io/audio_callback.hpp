#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <cstdint>
#include <functional>

namespace clover::io {

struct callback_args {
    const int_fast64_t clock_time;
    const int chan_count_in;
    const int chan_count_out;
    const float *input;
    float *output;
};

enum struct callback_status { cont, end };
using callback          = std::function<callback_status(callback_args)>;
using complete_callback = std::function<void()>;

}  // namespace clover::io