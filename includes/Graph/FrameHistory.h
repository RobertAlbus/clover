#pragma once

#include <algorithm>

#include "Graph/Frame.h"

namespace Clover::Graph {

template<size_t __arity>
class FrameHistory
{
public:
    FrameHistory() : _hasNext(false) { }
    // FrameHistory(FrameHistory&&) = default;

    void push(Frame<__arity> frame)
    {
        if (!_hasNext) _next = frame;
        _hasNext = false;
        std::swap(current, _next);
        std::swap(_next, last);
    }

    void next(Frame<__arity> frame)
    {
        _hasNext = true;
        _next = frame;
    }

    Frame<__arity> current = {};
    Frame<__arity> last = {};

protected:
    Frame<__arity> _next = {};
    bool _hasNext;
};

}