#pragma once
#include <functional>
#include <vector>


typedef std::function<void(int)> ClockCallbackFn;

class SampleClock
{
public:
    SampleClock();
    int currentSample();
    void tick();
    void registerTickCallback(ClockCallbackFn fn);

private:
    static int _currentSample;
    static std::vector<ClockCallbackFn> callbacks;
};
