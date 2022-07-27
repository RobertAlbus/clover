#pragma once

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

private:
    Sample samples[MAX_ARITY];
};

/// add right frame to left frame.
///
Frame& operator+= (Frame& frame1, const Frame& frame2);