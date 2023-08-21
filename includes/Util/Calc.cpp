/*
 * /////////
 * // Clover
 * 
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 * 
 * Copyright (C) 2023 Rob W. Albus
 *  
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

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

float ltodb(float x) { return log10f(fabsf(x)) * 20.f; }

float dbtol(float x) { return powf(10.f, x / 20.f); }

float mtof(float x) { return 440.f * powf(2.f, (x - 69.f) / 12.f); }

float ftom(float x) { return 12.f * (logf(x / 220.f) / logf(2)) + 57.f; }

} // namespace Clover::Util::Calc
