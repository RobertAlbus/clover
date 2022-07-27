#include "frame.h"
#include "frameHistory.h"


FrameHistory::FrameHistory(int arity) :
    frames {Frame(arity), Frame(arity), Frame(arity)},
    _hasNext(false),
    _lastIndex(0),
    _currentIndex(1),
    _nextIndex(2)
{

}

void FrameHistory::tick()
{
    _lastIndex++;    _lastIndex    %= 3;
    _currentIndex++; _currentIndex %= 3;
    _nextIndex++;    _nextIndex    %= 3;
    if (_hasNext) _hasNext = false;
}

Frame& FrameHistory::last()
{
    return frames[_lastIndex];
}

Frame& FrameHistory::current()
{
    return frames[_currentIndex];
}

void FrameHistory::current(Frame f)
{
    tick();
    frames[_currentIndex] = f;
}

void FrameHistory::next(Frame f)
{
    _hasNext = true;
    frames[_nextIndex] = f;
}

bool FrameHistory::hasNext()
{
    return _hasNext;
}
