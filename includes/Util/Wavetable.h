#pragma once
#include <vector>
#include <cmath>
#include <math.h>

#include "Constants.h"


namespace Clover::Util::GenerateWavetable {

// linear interpolation to create a larger wavetable from a smaller wavetable
// check Wavetable::Generate::Tri to see it in use
std::vector<Sample> LerpTable(int outputSize, std::vector<Sample> rawTable) {
    double scaleFactor = (double)outputSize / (double)rawTable.size();

    std::vector<Sample> wavetable;
    wavetable.reserve(outputSize);
    for( int i=0; i < outputSize; i++ )
    {
        double scaledIndex = i/scaleFactor;
        int truncatedIndex = (int)scaledIndex;
        int nextIndex = truncatedIndex + 1;
        double weight =  scaledIndex - truncatedIndex;

        wavetable.emplace_back(
            std::lerp(
                rawTable[truncatedIndex],
                rawTable[nextIndex],
                weight
            )
        );
    }
    return wavetable;
}

// Generate a wavetable for a sine wave.
std::vector<Sample> Sine(int size) {
    std::vector<Sample> wavetable;
    wavetable.reserve(size);
    for( int i=0; i<size; i++ )
    {
        wavetable.emplace_back(
            (float) sin( ((double)i/(double)size) * M_PI * 2. )
        );
    }

    return wavetable;
}

// Generate a wavetable for a pulse wave.
// pulseWidth 0..1 inclusive
std::vector<Sample> Pulse(int size, float pulseWidth) {
    std::vector<Sample> wavetable;
    wavetable.reserve(size);
    for( int i=0; i < size; i++ )
    {
        wavetable.emplace_back(
            i < ((float)size * pulseWidth) ? 1. : -1.
        );
    }

    return wavetable;
}

// Generate a wavetable for a square wave.
std::vector<Sample> Square(int size) {
    return Pulse(size, 0.5);
}

// Generate a wavetable for a saw wave.
std::vector<Sample> Saw(int size) {
    std::vector<Sample> wavetable;
    wavetable.reserve(size);
    double increment = 2. / (double) size;
    for( int i=0; i < size; i++ )
    {
        wavetable.emplace_back(
            (float) 1. - (i * increment)
        );
    }

    return wavetable;
}

// Generate a wavetable for a triangle wave.
std::vector<Sample> Tri(int size) {
    std::vector<Sample> lerpTable = {0,1,0,-1,0};
    return LerpTable(size, lerpTable);
}

// Generate a wavetable for a white noise wave.
std::vector<Sample> NoiseWhite(int size) {
    std::vector<Sample> wavetable;
    wavetable.reserve(size);
    for( int i=0; i < size; i++ )
    {
        float noise = ((float)rand()) / (float)(RAND_MAX);
        noise = 2.0 * (noise - 0.5);
        wavetable.emplace_back(noise);
    }

    return wavetable;
}
}
