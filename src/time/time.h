#pragma once

typedef int Sample;

class Time
{
public:
    Time(int sampleRate, double bpm);
    Sample currentSample();
    Sample tick();

    const int sampleRate;

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
    Sample _currentSample;
    void initMusicTime();

};