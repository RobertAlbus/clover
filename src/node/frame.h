#pragma once
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <stdexcept>
#include <string>

#include "arity.h"

typedef double Sample;

class Frame : public Arity
{
public:
    Frame() : Arity() {}
    Frame(int _arity) : Arity(_arity) {}
    Frame(const Frame& frame) : Arity(frame.arity) {
        arity = frame.arity;
        for (int i = 0; i < arity; i++) {
            setSampleAtIndex(i, frame.getSampleAtIndex(i));
        }
    }

    Sample getSampleAtIndex(int i) const {
        if (i >= arity) {
            throw std::domain_error("index out of bounds");
        }

        return samples[i];
    }

    void setSampleAtIndex(int i, Sample s) {
        if (i >= arity) {
            throw std::domain_error("index out of bounds");
        }

        samples[i] = s;
    }

    /// Convert a frame to a new arity.
    /// The only supported conversion at this time is between arities 1 and 2.
    Frame convertArity(int arity) const {
        int targetArity = arity;
        int sourceArity = getArity();
        
        if (targetArity == sourceArity) return Frame((*this));

        if (!supportsConversionTo(targetArity)) 
        {
            std::string errorMsg;

            errorMsg.append("no implementation for converting from Frame arity ");
            errorMsg.append(std::to_string(sourceArity));
            errorMsg.append(" to Frame arity ");
            errorMsg.append(std::to_string(targetArity));

            throw std::domain_error(errorMsg);
        }

        Frame newFrame = Frame(targetArity);
        
        if (sourceArity == 2 && targetArity == 1) {
            newFrame.setSampleAtIndex(
                0,
                getSampleAtIndex(0)
                            +
                getSampleAtIndex(1)
            );
        }
        else if (sourceArity == 1 && targetArity == 2) {
            newFrame.setSampleAtIndex(0, getSampleAtIndex(0));
            newFrame.setSampleAtIndex(1, getSampleAtIndex(0));
        }

        return newFrame;
    }

private:
    bool supportsConversionTo(int targetArity) const {
        int supportedSourceArities[] = {1, 2};
        int supportedTargetArities[] = {1, 2};
        
        return
            std::any_of(
                std::begin(supportedSourceArities),
                std::end(supportedSourceArities),
                [&](int i) { 
                    return targetArity  == i; 
                }
            ) &&
            std::any_of(
                std::begin(supportedTargetArities),
                std::end(supportedTargetArities),
                [&](int i) { 
                    return arity  == i; 
                }
            );
    }

    Sample samples[10] = {0,0,0,0,0,0,0,0,0,0};
};

/// combine two frames additively.
///
Frame& operator+= (Frame& frame1, const Frame& frame2) {
    // arity pairity
    if (frame1.getArity() != frame2.getArity()) {
        int targetArity = (static_cast<const Frame>(frame1)).getArity();
        Frame frame2 = frame2.convertArity(frame1.getArity());
    }
    for (int i = 0; i < frame1.getArity(); i++) {
        Sample newValue = frame1.getSampleAtIndex(i) + frame2.getSampleAtIndex(i);
        frame1.setSampleAtIndex(i, newValue);
    }

    return frame1;
}
