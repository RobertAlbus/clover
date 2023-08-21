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

#include "SampleClock.h"

namespace Clover::Util {

class Time {
public:
  Time(float bpm, size_t sampleRate, SampleClock *clock);

  float currentQuat();
  float currentTrip();
  float currentBeat();
  float currentBar();
  float currentUnit(float samplesPerUnit);

  int currentQuat_i();
  int currentTrip_i();
  int currentBeat_i();
  int currentBar_i();
  int currentUnit_i(float samplesPerUnit);

  float bpm(float multiplier = 1.);

  float quat(float multiplier = 1.);
  float trip(float multiplier = 1.);
  float beat(float multiplier = 1.);
  float bar(float multiplier = 1.);

  int quat_i(float multiplier = 1.);
  int trip_i(float multiplier = 1.);
  int beat_i(float multiplier = 1.);
  int bar_i(float multiplier = 1.);

  SampleClock *sampleClock;

  // // for example, every N bars
  // bool isMultipleOfDuration(float duration) {
  //   float currentSample = (float)sampleClock->currentSample();
  //   float nextSample = currentSample + 1.;

  //   bool exactMatch = fmod(currentSample, duration) == 0.;

  //   // for non-perfect bpms, ie 159, we check if it would fall between this
  //   and
  //   // next sample
  //   bool inRange = fmod(currentSample, duration) > fmod(nextSample,
  //   duration);

  //   return exactMatch || inRange;
  // }

  // bool isCurrentUnit(float time) {
  //   return sampleClock->currentSample() == (int)time;
  // }

private:
  size_t _sampleRate;

  float _bpm;
  float _quat;
  float _trip;
  float _beat;
  float _bar;

  void initMusicTime();
};

} // namespace Clover::Util
