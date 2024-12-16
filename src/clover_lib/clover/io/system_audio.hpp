#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>
#include <vector>

namespace clover::io {
struct device {
    std::string name;
    int max_chan_count_in;
    int max_chan_count_out;

    float default_latency_ms_input_high;
    float default_latency_ms_input_low;
    float default_latency_ms_output_high;
    float default_latency_ms_output_low;
    float default_sample_rate;
    int index;
    int host_index;
    std::string host_name;

    std::string to_string();
};

struct system_audio_config {
    system_audio_config();

    std::vector<std::string> hosts;
    std::vector<device> devices;
    device default_input();
    device default_output();

    void to_string();
};

}  // namespace clover::io