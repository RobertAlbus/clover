#pragma once

#include "frame.h"

template<size_t __arity>
class FrameHistory
{
public:
    FrameHistory() { }

    void push(Frame<__arity> frame)
    {
        std::swap(current, last);
        current = frame;
    }

    Frame<__arity> current = {};
    Frame<__arity> last = {};
};
