#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <stdexcept>
#include <string>

#include "arity.h"
#include "constants.h"
#include "frame.h"


Frame::Frame() : Arity() {
    throw std::domain_error("Frame must be instantiated with arity. \n Try Frame(arity)");
}

Frame::Frame(int _arity) : Arity(_arity) {
    for (int i; i < MAX_ARITY; i++) {
        samples[i] = 0.;
    }
}

Frame::Frame(const Frame& frame) : Arity(frame.arity()) {
    _arity = frame.arity();
    for (int i = 0; i < arity(); i++) {
        setSampleAtIndex(i, frame.getSampleAtIndex(i));
    }
}

Sample Frame::getSampleAtIndex(int i) const {
    if (i >= arity()) {
        throw std::domain_error("index out of bounds");
    }

    return samples[i];
}

void Frame::setSampleAtIndex(int i, Sample s) {
    if (i >= arity()) {
        throw std::domain_error("index out of bounds");
    }

    samples[i] = s;
}

Frame Frame::convertArity(int targetArity) const {
    int sourceArity = arity();
    
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

bool Frame::supportsConversionTo(int targetArity) const {
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
                return arity() == i; 
            }
        );
}

Frame& operator+= (Frame& frame1, const Frame& frame2) {
    // arity pairity
    if (frame1.arity() != frame2.arity()) {
        int targetArity = (static_cast<const Frame>(frame1)).arity();
        Frame frame2 = frame2.convertArity(frame1.arity());
    }
    for (int i = 0; i < frame1.arity(); i++) {
        Sample newValue = frame1.getSampleAtIndex(i) + frame2.getSampleAtIndex(i);
        frame1.setSampleAtIndex(i, newValue);
    }

    return frame1;
}