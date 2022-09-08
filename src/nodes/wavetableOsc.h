#pragma once
#include <functional>

#include "constants.h"
#include "wavetable.h"

template <size_t __tableSize, auto __wtFunct>
class WavetableOsc : public Node<0,1>
{
public:
    WavetableOsc() : Node<0,1>()
    {
        freq(100);
        phase(0);
        /* initialise sinusoidal wavetable */
        auto wt = __wtFunct();
        std::swap(wt, wavetable); 
    }

    // Assign waveform phase with value between 0.0 and 1.0
    void phase(float p) {
        p = fmod(fabs(p), 1.);
        _phase = p * __tableSize *_phaseIncrement;
    }

    // Set waveform phase as value between 0.0 and 1.0
    float phase() {
        return _phase / __tableSize;
    }

    void freq(float freq) {
        freq = std::max(freq, (float)0.);
        _readonlyFreq = freq;
        _phaseIncrement = freq * ((float)__tableSize) / ((float)SAMPLE_RATE);
    }

    float freq() {
        return _readonlyFreq;
    }

    float lerp() {
        int truncatedIndex = static_cast<int>(_phase);
        int nextIndex = (truncatedIndex + 1);
        if (nextIndex >= __tableSize) {
            nextIndex %= __tableSize;
        }

        float nextIndexWeight = _phase - static_cast<float>(truncatedIndex);
        float truncatedIndexWeight = 1. - nextIndexWeight;

        Sample value = truncatedIndexWeight * wavetable[truncatedIndex] + 
            nextIndexWeight * wavetable[nextIndex];

        return value;
    }

protected:
    Frame<1> tick(Frame<0> input)
    {
        Sample value = lerp();

        _phase = fmod(_phase + _phaseIncrement, __tableSize);

        Frame<1> f {value};

        return f;
    }

    std::array<Sample, __tableSize> wavetable;
private:
    float _phase;
    float _phaseIncrement;
    float _readonlyFreq;
};

template <size_t __tableSize>
class SineSized : public WavetableOsc<__tableSize, Wavetable::Generate::Sine<__tableSize>>
{ };

class Sine : public SineSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class SquareSized : public WavetableOsc<__tableSize, Wavetable::Generate::Square<__tableSize>>
{ };

class Square : public SquareSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class SawSized : public WavetableOsc<__tableSize, Wavetable::Generate::Saw<__tableSize>>
{ };

class Saw : public SawSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class TriSized : public WavetableOsc<__tableSize, Wavetable::Generate::Tri<__tableSize>>
{ };

class Tri : public TriSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class NoiseWhiteSized : public WavetableOsc<__tableSize, Wavetable::Generate::NoiseWhite<__tableSize>>
{ };

class NoiseWhite : public NoiseWhiteSized<1000000> {};