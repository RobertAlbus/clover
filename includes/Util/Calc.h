#pragma once

#include <cmath>

namespace Clover::Util::Calc {

float freqBySemitoneDifference(float freq, float semitones);

float tension(float value, float tension);

float sign(float value);

float ltodb(float x);

float dbtol(float x);

float mtof(float x);

float ftom(float x);

} // namespace Clover::Util::Calc
