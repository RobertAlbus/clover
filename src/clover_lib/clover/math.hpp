#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include "clover/float.hpp"

namespace clover {

// perception map function [0,n]->[0,n] intersecting (0,0) and (n,n)
auto db_curve(clover_float x, clover_float n = 1.f) -> clover_float;
// perception map function [0,n]->[0,n] intersecting (0,0) and (n,n)
auto db_curve_inverse(clover_float y, clover_float n = 1.f) -> clover_float;
// linear coefficient to decibel
auto db_to_linear(clover_float x) -> clover_float;
// decibel to linear coefficient
auto linear_to_db(clover_float x) -> clover_float;

auto frequency_by_octave_difference(clover_float freq, clover_float octaves) -> clover_float;
auto frequency_by_semitone_difference(clover_float freq, clover_float semitones) -> clover_float;

auto frequency_to_midi(clover_float x) -> clover_float;
auto midi_to_frequency(clover_float x) -> clover_float;

// signum without zero
auto sign(clover_float x) -> clover_float;

// an asymtotic tension curve for x, a [-1,1]
auto tension(clover_float x, clover_float a) -> clover_float;
// see clover::tension(x,a)
auto tension_pos_a(clover_float x, clover_float a) -> clover_float;
// see clover::tension(x,a)
auto tension_neg_a(clover_float x, clover_float a) -> clover_float;

}  // namespace clover
