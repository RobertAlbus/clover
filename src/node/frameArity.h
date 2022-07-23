#pragma once
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <stdexcept>
#include <string>

#include "arity.h"
#include "frame.h"


template <int _arity>
class FrameArityBase : public Frame, public Arity
{
public:
    FrameArityBase() : Frame(), Arity(_arity) {}
};

template <int _arity>
class FrameArity : public FrameArityBase<_arity>
{
public:
    FrameArity() : FrameArityBase<_arity>() {}

    Sample samples[_arity];

    /// Convert a frame to a new arity.
    /// The only supported conversion at this time is between arities 1 and 2.
    static FrameArity<_arity> From(const FrameArityBase &sourceFrame) {
        int targetArity = _arity;
        int sourceArity = ((Arity)sourceFrame).arity;
        
        if (targetArity == sourceArity) return ((FrameArity<_arity>)sourceFrame);

        if (!((Arity)sourceFrame).isSupportedConversion(targetArity)) 
        {
            std::string errorMsg;

            errorMsg.append("no implementation for converting from Frame arity ");
            errorMsg.append(std::to_string(sourceArity));
            errorMsg.append(" to Frame arity ");
            errorMsg.append(std::to_string(targetArity));

            throw std::domain_error(errorMsg);
        }

        FrameArity<_arity> newFrame;
        
        if (sourceArity == 2 && targetArity == 1) {
            newFrame.samples[0] = sourceFrame.samples[0] + sourceFrame.samples[1];
        }
        else if (sourceArity == 1 && targetArity == 2) {
            newFrame.samples[0] = sourceFrame.samples[0];
            newFrame.samples[1] = sourceFrame.samples[0];
        }

        return newFrame;
    }
};

/// add multiple frames together additively.
/// can only add Frames with matching arities - convert with Frame<N>::From(frame) first if required.
template <int _arity>
FrameArity<_arity> operator+= (FrameArity<_arity> frame1, FrameArity<_arity> frame2) {

    // arity pairity

    auto combine = [](Sample a, Sample b) {
        return a + b;
    };
    if (frame1.arity == frame2.arity) {
        for (int i = 0; i < _arity; i++) {
            frame1.samples[i] += frame2.samples[i];
        }
    }
}
