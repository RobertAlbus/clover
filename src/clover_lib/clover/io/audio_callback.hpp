#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstdint>
#include <functional>

#include "clover/audio_buffer.hpp"

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

audio_buffer exec_callback(
        // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        io::callback audio_callback,
        int channels_out,
        int sample_rate,
        int_fast64_t duration);

}  // namespace clover::io