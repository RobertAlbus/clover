#pragma once

typedef double Sample;

/// Base class for N channel audio sample frames.
/// ATTENTION: If you inherit from this class you must also inherit from Arity.
class Frame
{
public:
    Frame() {

    }
    Sample samples[0];
};

