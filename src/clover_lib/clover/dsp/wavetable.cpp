// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstddef>
#include <vector>

#include "clover/dsp/wave.hpp"
#include "clover/dsp/wavetable.hpp"
#include "clover/num.hpp"

namespace clover::dsp::wavetable {

static std::vector<float> generate_table(float (*wave_fn)(float)) {
    std::vector<float> table(table_size);
    for (size_t i = 0; i < table_size; ++i) {
        float phase = static_cast<float>(i) / static_cast<float>(table_size) * num::pi_x2;
        table[i]    = wave_fn(phase);
    }
    return table;
}

const std::vector<float> sine   = generate_table(wave_sine);
const std::vector<float> square = generate_table(wave_square);
const std::vector<float> saw    = generate_table(wave_saw);
const std::vector<float> tri    = generate_table(wave_tri);
const std::vector<float> noise  = generate_table(wave_noise);

}  // namespace clover::dsp::wavetable
