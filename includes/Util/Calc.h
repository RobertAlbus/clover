#pragma once

#include <cmath>

namespace Clover::Util::Calc {

float LinearToLog(float linearValue, float minValue, float maxValue);

float scaleFreqByOctave(float linearModulation, float f_center,
                        float octave_range);

float octaveByFreqDifference(float f1, float f2);

float freqBySemitoneDifference(float freq, float semitones);

float tension(float value, float tension);

float sign(float value);

float ltodb(float x);

float dbtol(float x);

float mtof(float x);

float ftom(float x);

} // namespace Clover::Util::Calc
