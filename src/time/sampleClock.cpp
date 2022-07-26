#include "constants.h"
#include "sampleClock.h"
#include <stdio.h>


SampleClock::SampleClock()
{

};

int SampleClock::tick()
{

    _currentSample++;
    for (auto& callback : callbacks)
    {
        callback(_currentSample);
    }
    return _currentSample;
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
