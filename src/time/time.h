#pragma once
#include "sampleClock.h"

class Time
{
public:
    Time(double bpm);

    double currentQuat();
    double currentTrip();
    double currentBeat();
    double currentBar();
    double currentUnit(int unitInSamples);

    double bpm;
    double bps;
    int quat;
    int trip;
    int beat;
    int bar;

private:
    void initMusicTime();
    SampleClock clock;
};
