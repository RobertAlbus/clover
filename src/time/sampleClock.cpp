#include <stdio.h>
#include <vector>

#include "constants.h"
#include "sampleClock.h"


SampleClock::SampleClock()
{
    callbacks.reserve(10);
};

int SampleClock::tick()
{
    _currentSample++;
    for (int i = 0, end = callbacks.size(); i < end; i++)
    {
        callbacks[i](_currentSample);
    }
    return currentSample();
}

int SampleClock::currentSample()
{
    return _currentSample;
}

void SampleClock::registerTickCallback(ClockCallbackFn fn)
{
    callbacks.emplace_back(fn);
}

int SampleClock::_currentSample;
std::vector<ClockCallbackFn> SampleClock::callbacks;
