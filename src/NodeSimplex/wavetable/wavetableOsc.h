#pragma once
#include <functional>

#include "constants.h"
#include "util.h"

namespace Clover::NodeSimplex::Wavetable {

template <size_t __tableSize, auto __wtFunct>
class WavetableOsc : public Node<0,1>
{
public:
    WavetableOsc() : Node<0,1>()
    {
        freq(100);
        phase(0);
        auto wt = __wtFunct();
        std::swap(wt, wavetable); 
    }

    void phase(float p) {
        _phase = fmod(p, 1.) * __tableSize *_phaseIncrement;
    }

    float phase() { return _phase / __tableSize; }

    void phaseOffset(float offset) {
        _phaseOffset = fmod(offset, 1) * __tableSize *_phaseIncrement;
    }

    float phaseOffset() { return _phaseOffset / __tableSize; }

    void freq(float freq) {
        freq = std::max(freq, (float)0.);
        _phaseIncrement = freq * ((float)__tableSize) / ((float)SAMPLE_RATE);
    }

    float freq() {
        return _phaseIncrement * SAMPLE_RATE / __tableSize;
    }

protected:
    Frame<1> tick(Frame<0> input)
    {
        int truncatedIndex = static_cast<int>(_phase);
        int nextIndex = (truncatedIndex + 1) % __tableSize;
        float value = std::lerp(
            wavetable[truncatedIndex],
            wavetable[nextIndex],
            _phase - truncatedIndex
        );

        Frame<1> f {value};
        _phase = fmod(_phase + _phaseOffset + _phaseIncrement, __tableSize);

        return f;
    }

    std::array<Sample, __tableSize> wavetable;
private:
    float _phase;
    float _phaseOffset;
    float _phaseIncrement;
};

template <size_t __tableSize>
class SineSized : public WavetableOsc<__tableSize, Clover::Util::GenerateWavetable::Sine<__tableSize>>
{ };

class Sine : public SineSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class SquareSized : public WavetableOsc<__tableSize, Clover::Util::GenerateWavetable::Square<__tableSize>>
{ };

class Square : public SquareSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class SawSized : public WavetableOsc<__tableSize, Clover::Util::GenerateWavetable::Saw<__tableSize>>
{ };

class Saw : public SawSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class TriSized : public WavetableOsc<__tableSize, Clover::Util::GenerateWavetable::Tri<__tableSize>>
{ };

class Tri : public TriSized<DEFAULT_WAVETABLE_SIZE> {};

template <size_t __tableSize>
class NoiseWhiteSized : public WavetableOsc<__tableSize, Clover::Util::GenerateWavetable::NoiseWhite<__tableSize>>
{ };

class NoiseWhite : public NoiseWhiteSized<1000000> {};

}