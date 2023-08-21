#pragma once

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

#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T> struct IIRCoefficientStrategy {
  // clang-format off
  virtual IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> highPass(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> lowShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> highShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> peakingEQ(T center_freq_hz, T Q, T gain_db, T samplerate_hz) = 0;
  // clang-format on
};

} // namespace Clover::Filter
