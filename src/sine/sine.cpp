#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "constants.h"
#include "node.h"
#include "sine.h"

#define TABLE_SIZE   (200)

Sine::Sine() : Node(0,2)
{
    freq(100);
    phase(0);
    /* initialise sinusoidal wavetable */
    for( int i=0; i<TABLE_SIZE; i++ )
    {
        wavetable[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
}

Frame Sine::tick(Frame input)
{
    Sample value = lerp();

    _phase = fmod(_phase + _phaseIncrement, TABLE_SIZE);

    Frame f = Frame(2);
    f.setSampleAtIndex(0, value);
    f.setSampleAtIndex(1, value);

    return f;
}

void Sine::freq(float freq) {
    freq = std::max(freq, (float)0.);
    _readonlyFreq = freq;
    _phaseIncrement = freq * ((float)TABLE_SIZE) / ((float)SAMPLE_RATE);
}

float Sine::freq() {
    return _readonlyFreq;
}

void Sine::phase(float p) {
    p = fmod(fabs(p), 1.);
    _phase = p * TABLE_SIZE *_phaseIncrement;
}

float Sine::phase() {
    return _phase / TABLE_SIZE;
}

float Sine::lerp() {
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