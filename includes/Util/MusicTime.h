#pragma once
#include "SampleClock.h"

namespace Clover::Util {

class Time
{
public:
    Time(float bpm, size_t sampleRate, SampleClock* clock) : bpm(bpm), sampleClock(clock)
    {
        _sampleRate = sampleRate;
        initMusicTime();
    }

    float currentQuat()
    {
        return currentUnit(quat);
    }

    float currentTrip()
    {
        return currentUnit(trip);
    }

    float currentBeat()
    {
        return currentUnit(beat);
    }

    float currentBar()
    {
        return currentUnit(bar);
    }
    
    float currentUnit(int samplesPerUnit)
    {
        return (float)sampleClock->currentSample() / (float)samplesPerUnit;
    }

    float bpm;
    int quat;
    int trip;
    int beat;
    int bar;

    SampleClock* sampleClock;
private:
    size_t _sampleRate;
    void initMusicTime() {
        float beatsPerSecond = bpm/60;

        beat = int((float)_sampleRate / beatsPerSecond);
        quat = beat / 4;
        trip = beat / 3;
        bar  = beat * 4;
    };
    
};

}