#pragma once

#include "frame.h"

class FrameHistory
{
public:
    FrameHistory(int arity);
    Frame& last();
    Frame& current();

    void current(Frame f);
    void next(Frame f);

    void tick();
    bool hasNext();

private:
    Frame frames[3];
    bool _hasNext;
    int _lastIndex;
    int _currentIndex;
    int _nextIndex; 
};
