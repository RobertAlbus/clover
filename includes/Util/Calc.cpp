#include <cmath>
#include "Calc.h"

namespace Clover::Util::Calc {

float freqBySemitoneDifference(float freq, float semitones) {
    return freq * powf(2.f, semitones / 12.f);
}

float tension(float value, float tension) { 
  return powf(fabsf(value), expf(tension * -1.f) * Calc::sign(value));
}

float sign(float value) { return value < 0. ? -1.f : 1.f; }

float ltodb(float x) { return log10f(fabs(x)) * 20.f; }

float dbtol(float x) { return powf(10.f, x / 20.f); }

float mtof(float x) { return 440.f * powf(2.f, (x - 69.f) / 12.f); }

float ftom(float x) { return 12.f * (logf(x / 220.f) / logf(2)) + 57.f; }

} // namespace Clover::Util::Calc
