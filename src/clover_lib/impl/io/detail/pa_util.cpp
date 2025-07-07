// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/io/detail/pa_util.hpp"
#include <fcntl.h>
#include <format>
#include <iostream>
#include <portaudio.h>
#include <stdexcept>
#include <unistd.h>

namespace clover::io::pa_util {

// a corresponding call to pa_terminate is required for each call to pa_initialize
void pa_initialize() {
    int saved_stderr = dup(STDERR_FILENO);
    int devnull      = ::open("/dev/null", O_RDWR);

    dup2(devnull, STDERR_FILENO);       // swallow port audio logging
    Pa_Initialize();                    //
    dup2(saved_stderr, STDERR_FILENO);  // unswallow port audio logging
}

// a corresponding call to pa_terminate is required for each call to pa_initialize
void pa_terminate() {
    pa_util::print_pa_error(Pa_Terminate());
}

void handle_pa_error(int err) {
    if (err != paNoError) {
        throw std::runtime_error(
                std::format(
                        "PaErrorCode {} {}\n\n"
                        "exiting...\n",
                        err,
                        Pa_GetErrorText(err)));
    }
}

void print_pa_error(int err) {
    if (err != paNoError) {
        std::cout << std::format(
                "PaErrorCode {} {}\n\n"
                "exiting...\n",
                err,
                Pa_GetErrorText(err));
    }
}

}  // namespace clover::io::pa_util