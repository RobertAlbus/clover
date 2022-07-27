#pragma once
#include <functional>

#include "container.h"

typedef std::function<void(int)> ClockCallbackFn;

class SampleClock
{
public:
    SampleClock();
    int currentSample();
    int tick();
    void registerTickCallback(ClockCallbackFn fn);

private:
    static int _currentSample;
    static Container<ClockCallbackFn, 10> callbacks;
};
