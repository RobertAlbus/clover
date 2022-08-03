#include <stdio.h>

#include "constants.h"
#include "container.h"
#include "sampleClock.h"


SampleClock::SampleClock()
{

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
    callbacks << fn;
}

int SampleClock::_currentSample;
Container<ClockCallbackFn, 10> SampleClock::callbacks;
