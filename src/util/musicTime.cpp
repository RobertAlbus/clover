#include "constants.h"
#include "musicTime.h"

Time::Time(float bpm) :
    bpm(bpm),
    bps(bpm/60.)
{
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

    beat = int((float)SAMPLE_RATE / beatsPerSecond);
    quat = beat / 4;
    trip = beat / 3;
    bar  = beat * 4;
};
