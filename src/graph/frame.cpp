#include <stdexcept>
#include <string>

#include "arity.h"
#include "constants.h"
#include "frame.h"


Frame::Frame(int _arity) : Arity(_arity), samples {0,0}
{

}

Frame::Frame(Sample s1, Sample s2) : Arity(2), samples {s1, s2}
{

}

Frame::Frame(Sample s) : Arity(1), samples {s, 0.}
{

}

Frame::Frame(const Frame& frame) : Arity(frame.arity)
{
    for (int i = 0; i < arity; ++i)
    {
        samples[i] = frame.samples[i];
    }
}

Sample Frame::getSampleAtIndex(int i) const
{
    // if (i >= arity) {
    //     throw std::domain_error("index out of bounds");
    // }

    return samples[i];
}

void Frame::setSampleAtIndex(int i, Sample s)
{
    // if (i >= arity) {
    //     throw std::domain_error("index out of bounds");
    // }

    samples[i] = s;
}

Frame& operator+= (Frame& frame1, const Frame& frame2)
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
