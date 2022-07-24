#pragma once

class SampleClock
{
public:
    SampleClock();
    int currentSample();
    int tick();

private:
    static int _currentSample;
};
