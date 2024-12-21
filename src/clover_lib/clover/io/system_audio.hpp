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

    double default_latency_ms_input_high;
    double default_latency_ms_input_low;
    double default_latency_ms_output_high;
    double default_latency_ms_output_low;
    double default_sample_rate;
    int index;
    int host_index;
    std::string host_name;

    auto to_string() -> std::string;
    void print();
};

struct system_audio_config {
    system_audio_config();

    std::vector<std::string> hosts;
    std::vector<device> devices;
    auto default_input() -> device;
    auto default_output() -> device;
    auto no_device() -> int;
    auto get_device(std::string name) -> device;

    void print();
};

}  // namespace clover::io