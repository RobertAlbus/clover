#pragma once
#include <cmath>
#include <functional>
#include <vector>

#include "constants.h"
#include "util.h"

namespace Clover::NodeSimplex::Wavetable {

typedef std::vector<Sample> Wavetable;

struct WavetableOscSettings {
    float freq;
    float phase;
    float phaseOffset;
    float readIndex;
    float readIndexIncrement;
    float readIndexOffset;
    Wavetable wavetable;
    float wavetableSize;
};

class WavetableOsc : public StatefulProcessor<0,1, WavetableOscSettings>
{
public:
    WavetableOsc() : WavetableOsc(
        Clover::Util::GenerateWavetable::Sine(400),
        200,
        0,
        0
    ) {}

    WavetableOsc(Wavetable wavetable, float freq, float phase, float phaseOffset) : StatefulProcessor<0,1, WavetableOscSettings>()
    {
        settings.current.wavetable, wavetable;
        settings.initial.wavetableSize = (float) wavetable.size();
        settings.initial.freq = freq;
        settings.initial.phase = normalizePhase(phase);
        settings.initial.phaseOffset = normalizePhase(phaseOffset);
        
        settings.initial.readIndexIncrement = calculateReadIndexIncrement(freq);
        settings.initial.readIndex = 0.;
        settings.initial.readIndexOffset = calculateReadIndexOffset(settings.initial.phaseOffset);

        settings.reset();
    }

    void phase(float phase) {
        phase = normalizePhase(phase);
        settings.current.phase = phase;
        settings.current.readIndex = ((float)settings.current.wavetableSize) * phase;
    }

    float phase() { return settings.current.phase / settings.current.wavetableSize; }

    void phaseOffset(float offset) {
        settings.current.phaseOffset = normalizePhase(offset);
        settings.current.readIndexOffset = calculateReadIndexOffset(offset);
    }

    float phaseOffset() { return settings.current.phaseOffset; }

    void freq(float freq) {
        settings.current.freq = freq;
        settings.current.readIndexIncrement = calculateReadIndexIncrement(freq);
    }

    float freq() {
        return settings.current.freq;
    }

    void wavetable(Wavetable wt) {
        settings.current.wavetable = wt;
        settings.current.wavetableSize = (float) wt.size();
    }

    Wavetable wavetable() { return settings.current.wavetable; }

    void sine(int size = 400)
    {
        wavetable(Clover::Util::GenerateWavetable::Sine(size));
    }
    void square(int size = 400)
    {
        wavetable(Clover::Util::GenerateWavetable::Square(size));
    }
    void saw(int size = 400)
    {
        wavetable(Clover::Util::GenerateWavetable::Saw(size));
    }
    void tri(int size = 400)
    {
        wavetable(Clover::Util::GenerateWavetable::Tri(size));
    }
    void noise(int size = 1000)
    {
        wavetable(Clover::Util::GenerateWavetable::NoiseWhite(size));
    }



private:
    Frame<1> tick(Frame<0> input)
    {
        float direction = Clover::Util::Calc::sign(settings.current.freq);
        float nextIndex = normalizeReadIndex(settings.current.readIndex + direction);
        float value = std::lerp(
            settings.current.wavetable[(int)settings.current.readIndex],
            settings.current.wavetable[nextIndex],
            settings.current.readIndex - nextIndex
        );

        Frame<1> f {value};

        float newIndex =
            settings.current.readIndex
            + settings.current.readIndexOffset
            + settings.current.readIndexIncrement;

        settings.current.readIndex = normalizeReadIndex(newIndex);

        return f;
    }

    float normalizeReadIndex(float index)
    {
        float wtSize = (float)settings.current.wavetableSize;
        return fmod(fmod(index, wtSize) + wtSize, wtSize);
    }

    float normalizePhase(float phase)
    {
        return fmod(fmod(phase, 1.) + 1., 1);
    }
    
    float calculateReadIndexIncrement(float freq)
    {
        return freq * ((float)settings.current.wavetableSize) / ((float)SAMPLE_RATE);
    }

    float calculateReadIndexOffset(float phaseOffset)
    {
        return ((float) settings.current.wavetableSize) * normalizePhase(phaseOffset);
    }
};

}