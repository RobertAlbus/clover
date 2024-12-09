#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "clover/float.hpp"

namespace clover {

auto frequency_by_octave_difference(clover_float freq, clover_float octaves) -> clover_float;
auto frequency_by_semitone_difference(clover_float freq, clover_float semitones) -> clover_float;
auto tension(clover_float value, clover_float tension) -> clover_float;
auto linear_to_db(clover_float x) -> clover_float;
auto db_to_linear(clover_float x) -> clover_float;
auto db_gain_map(clover_float x) -> clover_float;
auto db_gain_map_inverse(clover_float y) -> clover_float;
auto midi_to_frequency(clover_float x) -> clover_float;
auto frequency_to_midi(clover_float x) -> clover_float;

}  // namespace clover
