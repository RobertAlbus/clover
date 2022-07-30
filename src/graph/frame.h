#pragma once

#include <stdexcept>
#include <string>

#include "arity.h"
#include "constants.h"

template <int __arity>
class Frame : public Arity<__arity>
{
public:
    Frame() : samples {}
    {

    }

    Frame(std::array<Sample, __arity> samples)
    : Arity<__arity>(),
    samples { samples }
    {

    }

    Sample getSampleAtIndex(int i) const
    {
        // if (i >= arity) {
        //     throw std::domain_error("index out of bounds");
        // }

        return samples[i];
    }

    void setSampleAtIndex(int i, Sample s)
    {
        // if (i >= arity) {
        //     throw std::domain_error("index out of bounds");
        // }

        samples[i] = s;
    }

private:
    std::array<Sample, __arity> samples {};
};

/// add right frame to left frame.
///
template <int __arity>
Frame<__arity>& operator+= (Frame<__arity>& frame1, const Frame<__arity>& frame2)
{
    if (frame1.arity != frame2.arity) {
        throw std::domain_error("cannot add frames with mistmatched arities");
    }
    for (int i = 0, end = frame1.arity; i < end; i++) {
        Sample newValue = frame1.getSampleAtIndex(i) + frame2.getSampleAtIndex(i);
        frame1.setSampleAtIndex(i, newValue);
    }

    return frame1;
}

template <int __arity>
Frame<__arity>& operator*= (Frame<__arity>& frame, float gain)
{

    for (int i = 0, end = frame.arity; i < end; i++) {
        frame.setSampleAtIndex(i, frame.getSampleAtIndex(i) * gain);
    }

    return frame;
}
