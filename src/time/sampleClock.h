#pragma once
#include <vector>
#include <functional>

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
    static std::vector<ClockCallbackFn> callbacks;
};
