#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.


namespace clover {

// perception map function [0,n]->[0,n] intersecting (0,0) and (n,n)
auto db_curve(float x, float n = 1.f) -> float;
// perception map function [0,n]->[0,n] intersecting (0,0) and (n,n)
auto db_curve_inverse(float y, float n = 1.f) -> float;
// linear coefficient to decibel
auto db_to_linear(float x) -> float;
// decibel to linear coefficient
auto linear_to_db(float x) -> float;

auto frequency_by_octave_difference(float freq, float octaves) -> float;
auto frequency_by_semitone_difference(float freq, float semitones) -> float;

auto octave_difference_by_frequency(float base_freq, float target_freq) -> float;
auto semitone_difference_by_frequency(float base_freq, float target_freq) -> float;

auto frequency_to_midi(float x) -> float;
auto midi_to_frequency(float x) -> float;

// signum without zero
auto sign(float x) -> float;

// an asymtotic tension curve for x, a [-1,1]
auto tension(float x, float a) -> float;
// see clover::tension(x,a)
auto tension_pos_a(float x, float a) -> float;
// see clover::tension(x,a)
auto tension_neg_a(float x, float a) -> float;

}  // namespace clover
