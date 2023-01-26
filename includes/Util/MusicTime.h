#pragma once
#include "SampleClock.h"

namespace Clover::Util {

class Time {
public:
  Time(float bpm, size_t sampleRate, SampleClock *clock)
      : _bpm(bpm), sampleClock(clock) {
    _sampleRate = sampleRate;
    initMusicTime();
  }

  float currentQuat() { return currentUnit(_quat); }
  float currentTrip() { return currentUnit(_trip); }
  float currentBeat() { return currentUnit(_beat); }
  float currentBar() { return currentUnit(_bar); }
  float currentUnit(float samplesPerUnit) {
    return ((float)sampleClock->currentSample()) / samplesPerUnit;
  }

  int currentQuat_i() { return (int)currentUnit(_quat); }
  int currentTrip_i() { return (int)currentUnit(_trip); }
  int currentBeat_i() { return (int)currentUnit(_beat); }
  int currentBar_i() { return (int)currentUnit(_bar); }
  int currentUnit_i(float samplesPerUnit) {
    return (int)currentUnit(samplesPerUnit);
  }

  float bpm(float multiplier = 1.) { return _bpm * multiplier; }

  float quat(float multiplier = 1.) { return _quat * multiplier; }
  float trip(float multiplier = 1.) { return _trip * multiplier; }
  float beat(float multiplier = 1.) { return _beat * multiplier; }
  float bar(float multiplier = 1.) { return _bar * multiplier; }

  float quat_i(float multiplier = 1.) { return (int)quat(multiplier); }
  float trip_i(float multiplier = 1.) { return (int)trip(multiplier); }
  float beat_i(float multiplier = 1.) { return (int)beat(multiplier); }
  float bar_i(float multiplier = 1.) { return (int)bar(multiplier); }

  SampleClock *sampleClock;

private:
  size_t _sampleRate;

  float _bpm;
  float _quat;
  float _trip;
  float _beat;
  float _bar;

  void initMusicTime() {
    float beatsPerSecond = _bpm / 60.;

    _beat = (float)_sampleRate / beatsPerSecond;
    _quat = _beat / 4.;
    _trip = _beat / 3.;
    _bar = _beat * 4.;
  };
};

} // namespace Clover::Util