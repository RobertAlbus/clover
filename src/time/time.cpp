#include "constants.h"
#include "time.h"

Time::Time(double bpm) :
    bpm(bpm),
    bps(bpm/60.)
{
    initMusicTime();
};

double Time::currentUnit(int unitInSamples)
{
    return clock.currentSample() / unitInSamples;
}

double Time::currentQuat()
{
    return currentUnit(quat);
}

double Time::currentTrip()
{
    return currentUnit(trip);
}

double Time::currentBeat()
{
    return currentUnit(beat);
}

double Time::currentBar()
{
    return currentUnit(bar);
}


void Time::initMusicTime() {
    double beatsPerSecond = bpm/60;

    beat = int((double)SAMPLE_RATE / beatsPerSecond);
    quat = beat / 4;
    trip = beat / 3;
    bar  = beat * 4;
};
