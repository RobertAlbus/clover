#pragma once

#include "frame.h"

template<int __arity>
class FrameHistory
{
public:
    FrameHistory() :
        _lastIndex(0),
        _currentIndex(1)
    {

    }

    Frame<__arity>& last()
    {
        return frames[_lastIndex];
    }

    Frame<__arity>& current()
    {
        return frames[_currentIndex];
    }

    void current(Frame<__arity> f)
    {
        tick();
        frames[_currentIndex] = f;
    }

    void tick()
    {
        _lastIndex++;    _lastIndex    %= 2;
    }

private:
    std::array<Frame<__arity>, 2> frames;
    int _lastIndex;
    int _currentIndex;
};
