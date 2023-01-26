#pragma once
#include "SampleClock.h"

namespace Clover::Util {

class Time {
public:
  Time(float bpm, size_t sampleRate, SampleClock *clock)
      : bpm(bpm), sampleClock(clock) {
    _sampleRate = sampleRate;
    initMusicTime();
  }

  float currentQuat() { return currentUnit(quat); }
  float currentTrip() { return currentUnit(trip); }
  float currentBeat() { return currentUnit(beat); }
  float currentBar() { return currentUnit(bar); }
  float currentUnit(float samplesPerUnit) {
    return ((float)sampleClock->currentSample()) / samplesPerUnit;
  }

  int currentQuat_i() { return (int) currentUnit(quat); }
  int currentTrip_i() { return (int) currentUnit(trip); }
  int currentBeat_i() { return (int) currentUnit(beat); }
  int currentBar_i() { return (int) currentUnit(bar); }
  int currentUnit_i(float samplesPerUnit) {
    return (int) currentUnit(samplesPerUnit);
  }

  float bpm;
  float quat;
  float trip;
  float beat;
  float bar;

  SampleClock *sampleClock;

private:
  size_t _sampleRate;
  void initMusicTime() {
    float beatsPerSecond = bpm / 60.;

    beat = (float)_sampleRate / beatsPerSecond;
    quat = beat / 4.;
    trip = beat / 3.;
    bar = beat * 4.;
  };
};

} // namespace Clover::Util