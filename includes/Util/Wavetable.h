#pragma once
#include <cmath>
#include <math.h>
#include <vector>

#include "Constants.h"

namespace Clover::Util::GenerateWavetable {

// linear interpolation to create a larger wavetable from a smaller wavetable
// check Wavetable::Generate::Tri to see it in use
std::vector<Sample> LerpTable(int outputSize, std::vector<Sample> rawTable);

// Generate a wavetable for a sine wave.
std::vector<Sample> Sine(int size);

// Generate a wavetable for a pulse wave.
// pulseWidth 0..1 inclusive
std::vector<Sample> Pulse(int size, float pulseWidth);

// Generate a wavetable for a square wave.
std::vector<Sample> Square(int size);

// Generate a wavetable for a saw wave.
std::vector<Sample> Saw(int size);

// Generate a wavetable for a triangle wave.
std::vector<Sample> Tri(int size);

// Generate a wavetable for a white noise wave.
std::vector<Sample> NoiseWhite(int size);

} // namespace Clover::Util::GenerateWavetable
