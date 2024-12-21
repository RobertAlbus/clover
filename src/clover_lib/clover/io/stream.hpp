#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstring>
#include <iostream>
#include <semaphore>
#include <unistd.h>

#include "clover/io/audio_callback.hpp"
#include "clover/io/sample_clock.hpp"

#include "portaudio.h"

namespace clover::io {

struct stream {
    struct settings {
        int device_index_in;
        int chan_count_in;

        int device_index_out;
        int chan_count_out;

        int sample_rate;
        double latency_ms;
    };

    stream();
    ~stream();

    void start();
    void wait_to_complete();
    void stop();
    void open(stream::settings settings);

    callback audio_callback = [](callback_args args) -> callback_status {
        std::cout << "clover::io::stream::audio_callback invoked without being set...." << std::endl;
        return callback_status::end;
    };

    complete_callback audio_complete_callback = [&]() {
        std::cout << "clover::io::stream::audio_complete_callback" << std::endl;
    };

   private:  //  don't want to mess with this
    settings current_settings;
    void *current_stream;
    std::binary_semaphore completion_signal;
    sample_clock clock;

    static void pa_stream_complete_callback(void *userData);
    static int pa_audio_callback(
            const void *input_buffer,
            void *output_buffer,
            unsigned long frames_per_buffer,
            const PaStreamCallbackTimeInfo *time_info,
            PaStreamCallbackFlags status_flags,
            void *user_data);

    void handle_pa_error(int err);
    void print_pa_error(int err);
};

}  // namespace clover::io
