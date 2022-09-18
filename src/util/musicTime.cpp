#include "musicTime.h"

using namespace Clover::Util;

Time::Time(float bpm, size_t sampleRate) :
    bpm(bpm)
{
    _sampleRate = sampleRate;
    initMusicTime();
};

float Time::currentUnit(int samplesPerUnit)
{
    return (float)sampleClock.currentSample() / (float)samplesPerUnit;
}

float Time::currentQuat()
{
    return currentUnit(quat);
}

float Time::currentTrip()
{
    return currentUnit(trip);
}

float Time::currentBeat()
{
    return currentUnit(beat);
}

float Time::currentBar()
{
    return currentUnit(bar);
}

void Time::initMusicTime() {
    float beatsPerSecond = bpm/60;

    beat = int((float)_sampleRate / beatsPerSecond);
    quat = beat / 4;
    trip = beat / 3;
    bar  = beat * 4;
};
