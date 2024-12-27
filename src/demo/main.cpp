// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/oscillator.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"
#include "clover/math.hpp"

#include "clover/circular_view.hpp"
#include <concepts>

bool PLAYBACK = false;
bool RENDER   = true;

using namespace clover;
// int main(int argc, char *argv[]) {
//     float fs              = 48000;
//     int fs_i              = static_cast<int>(fs);
//     int duration          = 5 * fs_i;
//     int channel_count_out = 2;

//     dsp::oscillator osc(fs);
//     dsp::oscillator mod(fs);

//     auto audio_state_init = [&]() {
//         osc.freq(400);
//         mod.freq(2);
//         osc.phase(0);
//         mod.phase(0);
//     };

//     auto audio_callback = [&](io::callback_args data) {
//         float &L = *(data.output);
//         float &R = *(data.output + 1);

//         L = osc.tick();
//         R = L;

//         float freq = frequency_by_octave_difference(400, (7.f / 12.f) * mod.tick());
//         osc.freq(freq);

//         if (data.clock_time == duration) {
//             return io::callback_status::end;
//         }
//         return io::callback_status::cont;
//     };

//     if (PLAYBACK) {
//         io::system_audio_config system;
//         system.print();
//         io::stream stream;
//         stream.audio_callback = audio_callback;

//         audio_state_init();
//         stream.open(io::stream::settings{
//                 .device_index_in  = system.no_device(),
//                 .chan_count_in    = 0,
//                 .device_index_out = system.default_output().index,
//                 .chan_count_out   = channel_count_out,
//                 .sample_rate      = fs_i,
//                 .latency_ms       = 0});

//         stream.start();
//         stream.wait_to_complete();
//         stream.stop();
//     }

//     if (RENDER) {
//         audio_state_init();
//         audio_buffer render = io::exec_callback(audio_callback, channel_count_out, fs_i, duration);
//         io::audio_file::write("test.wav", render, io::audio_file_settings::wav_441_16);
//     }
// }

int main(int argc, char* argv[]) {
    std::vector<float> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    circular_view circ{vec.begin(), vec.end()};

    auto iter = circ.begin();

    std::cout << "start iota" << std::endl;

    for (auto x : std::views::iota(0, 20)) {
        std::cout << (*iter) << "  " << std::endl;
        iter++;
    }

    std::cout << "done iota" << std::endl;
    std::cout << "start default range" << std::endl;

    for (auto& x : circ) {
        x = x * 2;
    }

    for (auto& x : circ) {
        std::cout << x << std::endl;
    }

    for (auto& x : circ) {
        x = x / 2;
    }

    std::cout << "done default range" << std::endl;

    std::cout << "start custom range" << std::endl;

    for (auto& x : circ.from(-2)) {
        std::cout << x << std::endl;
    }

    std::cout << "done custom range" << std::endl;
    std::cout << "size: " << circ.size() << std::endl;
}
