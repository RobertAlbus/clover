// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>  // IWYU pragma: keep
#include <iostream>
#include <string>

#include "portaudio.h"

#include "clover/io/system_audio.hpp"

namespace clover::io {

auto device::to_string() -> std::string {
    // clang-format off
    return std::format(""
        "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
        "┃ {}\n"
        "┣━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
        "┃ {:<19} ┃ {:<7}    {:<13} \n"
        "┃ {:<19} ┃ {:<7}    {:<13} \n"
        "┃ {:<19} ┃ {:<7} ms {:<13} \n"
        "┃ {:<19} ┃ {:<7} ms {:<13} \n"
        "┃ {:<19} ┃ {:<7} ms {:<13} \n"
        "┃ {:<19} ┃ {:<7} ms {:<13} \n"
        "┃ {:<19} ┃ {:<7} hz {:<13} \n"
        "┃ {:<19} ┃ {:<7}    {:<13} \n"
        "\n",
        name,
        "Max Channels In", max_chan_count_in, "",
        "Max Channels Out", max_chan_count_out, "",
        "Latency In High", default_latency_ms_input_high, "",
        "Latency In Low", default_latency_ms_input_low, "",
        "Latency Out High", default_latency_ms_output_high, "",
        "Latency Out Low", default_latency_ms_output_low, "",
        "Default Sample Rate", static_cast<int>(default_sample_rate), "",
        "Device Index", index, ""
    );
    // clang-format on
}

system_audio_config::system_audio_config() {
    const int host_api_count = Pa_GetHostApiCount();
    hosts.reserve(host_api_count);

    for (int i = 0; i < host_api_count; ++i) {
        hosts.emplace_back(Pa_GetHostApiInfo(i)->name);
    }

    const int device_count = Pa_GetDeviceCount();
    devices.reserve(device_count);

    for (int i = 0; i < device_count; ++i) {
        const PaDeviceInfo *current_device = Pa_GetDeviceInfo(i);
        devices.emplace_back(
                device{.name                           = current_device->name,
                       .max_chan_count_in              = current_device->maxInputChannels,
                       .max_chan_count_out             = current_device->maxOutputChannels,
                       .default_latency_ms_input_high  = (float)current_device->defaultHighInputLatency,
                       .default_latency_ms_input_low   = (float)current_device->defaultLowInputLatency,
                       .default_latency_ms_output_high = (float)current_device->defaultHighOutputLatency,
                       .default_latency_ms_output_low  = (float)current_device->defaultLowOutputLatency,
                       .default_sample_rate            = (float)current_device->defaultSampleRate,
                       .index                          = i,
                       .host_index                     = current_device->hostApi,
                       .host_name                      = hosts[current_device->hostApi]});
    }
}

device system_audio_config::default_input() {
    return devices[Pa_GetDefaultInputDevice()];
}
device system_audio_config::default_output() {
    return devices[Pa_GetDefaultOutputDevice()];
}

void system_audio_config::to_string() {
    for (auto device : devices) {
        std::cout << device.to_string() << std::endl;  // does it need endl?
    }
}

}  // namespace clover::io