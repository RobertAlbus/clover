#include "constants.h"
#include "sampleClock.h"

SampleClock::SampleClock()
{

};

int SampleClock::tick()
{
    return ++_currentSample;
}

int SampleClock::currentSample()
{
    return _currentSample;
}
