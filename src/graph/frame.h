#pragma once

#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <stdexcept>
#include <string>

#include "arity.h"
#include "constants.h"


class Frame : public Arity
{
public:
    Frame(int _arity);
    Frame(Sample s1, Sample s2);
    Frame(Sample s);
    Frame(const Frame& frame);

    Sample getSampleAtIndex(int i) const;
    void setSampleAtIndex(int i, Sample s);

    /// Convert a frame to a new arity.
    /// The only supported conversion at this time is between arities 1 and 2.
    Frame convertArity(int targetArity) const;

private:
    Sample samples[MAX_ARITY];
};

/// combine two frames additively.
///
Frame& operator+= (Frame& frame1, const Frame& frame2);