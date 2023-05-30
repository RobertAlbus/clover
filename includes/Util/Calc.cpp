#include <cmath>
#include "Calc.h"

namespace Clover::Util::Calc {

float linearToLog(float linearValue, float minValue, float maxValue) {
    float logMinValue = std::log2(minValue);
    float logMaxValue = std::log2(maxValue);

    float logValue = linearValue * (logMaxValue - logMinValue) + logMinValue;

    return std::pow(2.f, logValue);
}

// this is incorrecto
// float LinearLogCurve(float value, float minValue, float maxValue) {
//     const float exponent = 2.3026f; // log10(10.0f)
//     const float scaleFactor = 0.11111111f; // Reciprocal of 1/9

//     float scaledValue = (std::exp(exponent * value) - 1.0f) * scaleFactor;
//     return scaledValue * (maxValue - minValue) + minValue;
// }

float scaleFreqByOctave(float linearModulation, float f_center, float octave_range) {
    return f_center * powf(2.f, octave_range * linearModulation);
}

float octaveByFreqDifference(float centerFreq, float newFreq) {
    return log2f(newFreq / centerFreq);
}

float freqBySemitoneDifference(float freq, float semitones) {
    return freq * pow(2.0, semitones / 12.0);
}

float tension(float value, float tension) { 
  return pow(fabs(value),exp(tension * -1.f) * Calc::sign(value));
}

float sign(float value) { return value < 0. ? -1 : 1; }

float ltodb(float x) { return log10(fabs(x)) * 20.; }

float dbtol(float x) { return powf32(10., x / 20.); }

float mtof(float x) { return 440. * powf32(2., (x - 69.) / 12.); }

float ftom(float x) { return 12. * (log(x / 220.) / log(2)) + 57.; }

} // namespace Clover::Util::Calc
