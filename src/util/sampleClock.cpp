#include <stdio.h>
#include <vector>

#include "sampleClock.h"

using namespace Clover::Util;

SampleClock::SampleClock()
{
    callbacks.reserve(10);
    _currentSample = 0;
};

void SampleClock::tick()
{
    for (int i = 0, end = callbacks.size(); i < end; i++)
    {
        callbacks[i](_currentSample);
    }
    _currentSample++;
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
