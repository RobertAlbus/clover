#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <memory>

#include "clover/io/audio_callback.hpp"

namespace clover::io {

struct stream {
    struct config {
        int sample_rate  = 48000;
        int channels_in  = 0;  // 0 = input disabled
        int channels_out = 2;  // 0 = output disabled
    };

    explicit stream(config cfg);
    ~stream();

    stream(const stream&)            = delete;
    stream& operator=(const stream&) = delete;

    callback audio_callback;
    complete_callback audio_complete_callback;

    void start();
    void stop();
    void wait_to_complete();

    bool is_running() const;
    int negotiated_sample_rate() const;

   private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

}  // namespace clover::io
