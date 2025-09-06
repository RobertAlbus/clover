// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>  // IWYU pragma: keep

#include "portaudio.h"

#include "clover/io/audio_callback.hpp"
#include "clover/io/detail/pa_util.hpp"
#include "clover/io/stream.hpp"

namespace clover::io {

stream::stream() : current_stream(nullptr), completion_signal(0) {
    pa_util::pa_initialize();
}

stream::~stream() {
    if (current_stream != nullptr) {
        pa_util::print_pa_error(Pa_StopStream(current_stream));
        pa_util::print_pa_error(Pa_CloseStream(current_stream));
        current_stream = nullptr;
    }

    pa_util::pa_terminate();
}

void stream::start() {
    if (current_stream == nullptr) {
        throw std::runtime_error(
                std::format(
                        "clover::stream::start() called before "
                        "clover::stream::open(device)\n"
                        "exiting..."));
    }
    pa_util::handle_pa_error(Pa_IsStreamActive(current_stream));
    pa_util::handle_pa_error(Pa_StartStream(current_stream));
}

void stream::wait_to_complete() {
    completion_signal.acquire();
}

void stream::stop() {
    if (current_stream != nullptr) {
        pa_util::handle_pa_error(Pa_StopStream(current_stream));
        current_stream = nullptr;
    }
}

void stream::open(stream::settings settings) {
    current_settings = settings;
    bool has_input   = current_settings.device_index_in != paNoDevice;
    bool has_output  = current_settings.device_index_out != paNoDevice;

    if (!has_input && !has_output) {
        std::cout << "clover::io::stream::open requires non-zero value for at least one of:\n"
                  << "- chan_count_in\n"
                  << "- chan_count_out" << std::endl;
        return;
    }

    PaStreamParameters *pa_in_params  = nullptr;
    PaStreamParameters *pa_out_params = nullptr;

    if (has_input) {
        pa_in_params                            = new PaStreamParameters();
        pa_in_params->device                    = current_settings.device_index_in;
        pa_in_params->channelCount              = current_settings.chan_count_in;
        pa_in_params->sampleFormat              = paFloat32;
        pa_in_params->suggestedLatency          = current_settings.latency_ms;
        pa_in_params->hostApiSpecificStreamInfo = nullptr;
    }
    if (has_output) {
        pa_out_params                            = new PaStreamParameters();
        pa_out_params->device                    = current_settings.device_index_out;
        pa_out_params->channelCount              = current_settings.chan_count_out;
        pa_out_params->sampleFormat              = paFloat32;
        pa_out_params->suggestedLatency          = current_settings.latency_ms;
        pa_out_params->hostApiSpecificStreamInfo = nullptr;
    }

    pa_util::handle_pa_error(Pa_IsFormatSupported(pa_in_params, pa_out_params, current_settings.sample_rate));

    pa_util::handle_pa_error(Pa_OpenStream(
            &current_stream,
            pa_in_params,
            pa_out_params,
            current_settings.sample_rate,
            paFramesPerBufferUnspecified,
            paNoFlag,
            &stream::pa_audio_callback,
            this));

    pa_util::handle_pa_error(
            Pa_SetStreamFinishedCallback(current_stream, &stream::pa_stream_complete_callback));
    delete pa_in_params;
    delete pa_out_params;
}

void stream::pa_stream_complete_callback(void *user_data) {
    stream &instance = *(stream *)user_data;

    instance.audio_complete_callback();
    instance.completion_signal.release();
}

int stream::pa_audio_callback(
        const void *input_buffer,
        void *output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo *time_info,
        PaStreamCallbackFlags status_flags,
        void *user_data) {
    stream &instance = *(stream *)user_data;

    auto *out       = (float *)output_buffer;
    const float *in = (float *)input_buffer;

    std::fill_n(out, instance.current_settings.chan_count_out * frames_per_buffer, 0.f);

    for (unsigned long i = 0; i < frames_per_buffer; i++) {
        float *current_out     = out;
        callback_status status = instance.audio_callback(
                callback_args{
                        .clock_time     = instance.clock.current_sample(),
                        .chan_count_in  = instance.current_settings.chan_count_in,
                        .chan_count_out = instance.current_settings.chan_count_out,
                        .input          = (float *)in,
                        .output         = current_out,
                });

        switch (status) {
            case callback_status::cont:
                in += instance.current_settings.chan_count_in;
                out += instance.current_settings.chan_count_out;
                instance.clock.tick();
                break;
            case callback_status::end:
                return paComplete;
        }
    }

    return paContinue;
}

}  // namespace clover::io