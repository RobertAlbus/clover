#pragma once

#include "node.h"
#include "wavetable.h"

#define TABLE_SIZE   (200)

class Square : public Node<0,1>
{
public:
    Square() : Node<0,1>()
    {
        freq(100);
        phase(0);
        /* initialise sinusoidal wavetable */
        auto wt = Wavetable::Generate::Square<TABLE_SIZE>();
        std::swap(wt, wavetable); 
    }

    // Assign waveform phase with value between 0.0 and 1.0
    void phase(float p) {
        p = fmod(fabs(p), 1.);
        _phase = p * TABLE_SIZE *_phaseIncrement;
    }

    // Set waveform phase as value between 0.0 and 1.0
    float phase() {
        return _phase / TABLE_SIZE;
    }

    void freq(float freq) {
        freq = std::max(freq, (float)0.);
        _readonlyFreq = freq;
        _phaseIncrement = freq * ((float)TABLE_SIZE) / ((float)SAMPLE_RATE);
    }

    float freq() {
        return _readonlyFreq;
    }

    float lerp() {
        int truncatedIndex = static_cast<int>(_phase);
        int nextIndex = (truncatedIndex + 1);
        if (nextIndex >= TABLE_SIZE) {
            nextIndex %= TABLE_SIZE;
        }

        float nextIndexWeight = _phase - static_cast<float>(truncatedIndex);
        float truncatedIndexWeight = 1. - nextIndexWeight;

        return truncatedIndexWeight * wavetable[truncatedIndex] + 
            nextIndexWeight * wavetable[nextIndex];
    }

protected:
    Frame<1> tick(Frame<0> input)
    {
        Sample value = lerp();

        _phase = fmod(_phase + _phaseIncrement, TABLE_SIZE);

        Frame<1> f;

        f.setSampleAtIndex(0, value);
        f.setSampleAtIndex(1, value);

        return f;
    }

    std::array<Sample, TABLE_SIZE> wavetable;
private:
    float _phase;
    float _phaseIncrement;
    float _readonlyFreq;
};

