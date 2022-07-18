#include "time.h"

Time::Time(int sampleRate, double bpm) :
    sampleRate(sampleRate),
    bpm(bpm),
    bps(bpm/60.),
    _currentSample(0) 
{
    initMusicTime();
};

Sample Time::tick()
{
    return ++_currentSample;
}

Sample Time::currentSample()
{
    return _currentSample;
}

double Time::currentUnit(int unitInSamples)
{
    return currentSample() / unitInSamples;
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

    beat = int((double)sampleRate / beatsPerSecond);
    quat = beat / 4;
    trip = beat / 3;
    bar  = beat * 4;
};
