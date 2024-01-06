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

#include "MusicTime.h"
#include "SampleClock.h"

namespace Clover::Util {

Time::Time(float bpm, size_t sampleRate, SampleClock *clock)
    : _bpm(bpm), sampleClock(clock) {
  _sampleRate = sampleRate;
  initMusicTime();
}

float Time::currentQuat() { return currentUnit(_quat); }
float Time::currentTrip() { return currentUnit(_trip); }
float Time::currentBeat() { return currentUnit(_beat); }
float Time::currentBar() { return currentUnit(_bar); }
float Time::currentUnit(float samplesPerUnit) {
  return ((float)sampleClock->currentSample()) / samplesPerUnit;
}

int Time::currentQuat_i() { return (int)currentUnit(_quat); }
int Time::currentTrip_i() { return (int)currentUnit(_trip); }
int Time::currentBeat_i() { return (int)currentUnit(_beat); }
int Time::currentBar_i() { return (int)currentUnit(_bar); }
int Time::currentUnit_i(float samplesPerUnit) {
  return (int)currentUnit(samplesPerUnit);
}

float Time::bpm(float multiplier) { return _bpm * multiplier; }

float Time::quat(float multiplier) { return _quat * multiplier; }
float Time::trip(float multiplier) { return _trip * multiplier; }
float Time::beat(float multiplier) { return _beat * multiplier; }
float Time::bar(float multiplier) { return _bar * multiplier; }

int Time::quat_i(float multiplier) { return (int)quat(multiplier); }
int Time::trip_i(float multiplier) { return (int)trip(multiplier); }
int Time::beat_i(float multiplier) { return (int)beat(multiplier); }
int Time::bar_i(float multiplier) { return (int)bar(multiplier); }

void Time::initMusicTime() {
  float beatsPerSecond = _bpm / 60.;

  _beat = (float)_sampleRate / beatsPerSecond;
  _quat = _beat / 4.;
  _trip = _beat / 3.;
  _bar = _beat * 4.;
};

} // namespace Clover::Util
