// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>  // IWYU pragma: keep
#include <iostream>
#include <stdexcept>
#include <string>

#include "portaudio.h"

#include "clover/io/system_audio.hpp"

namespace clover::io {

void device::print() {
    std::cout << to_string();
}

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
        const PaDeviceInfo* current_device = Pa_GetDeviceInfo(i);
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

auto system_audio_config::default_input() -> device {
    return devices[Pa_GetDefaultInputDevice()];
}

auto system_audio_config::default_output() -> device {
    return devices[Pa_GetDefaultOutputDevice()];
}

auto system_audio_config::no_device() -> int {
    return paNoDevice;
}

auto system_audio_config::get_device(std::string name) -> device {
    for (auto& device : devices)
        if (device.name.starts_with(name))
            return device;
    throw std::invalid_argument(std::format("no audio device available: {}", name.c_str()));
}

void system_audio_config::print() {
    for (auto device : devices) {
        std::cout << device.to_string();
    }
}

}  // namespace clover::io