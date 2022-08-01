#pragma once
#include "sampleClock.h"

class Time
{
public:
    Time(float bpm);

    float currentQuat();
    float currentTrip();
    float currentBeat();
    float currentBar();
    float currentUnit(int unitInSamples);

    float bpm;
    float bps;
    int quat;
    int trip;
    int beat;
    int bar;

private:
    void initMusicTime();
    SampleClock sampleClock;
};
