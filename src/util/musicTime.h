#pragma once
#include "sampleClock.h"

namespace Clover::Util {

class Time
{
public:
    Time(float bpm, size_t sampleRate);

    float currentQuat();
    float currentTrip();
    float currentBeat();
    float currentBar();
    float currentUnit(int unitInSamples);

    float bpm;
    int quat;
    int trip;
    int beat;
    int bar;

private:
    size_t _sampleRate;
    void initMusicTime();
    SampleClock sampleClock;
};

}