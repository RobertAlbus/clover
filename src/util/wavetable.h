#pragma once
#include <array>
#include <cmath>
#include <math.h>

#include "constants.h"

namespace Wavetable {
    namespace Generate {

        // linear interpolation to create a larger wavetable from a smaller wavetable
        // check Wavetable::Generate::Tri to see it in use
        template<size_t __tableSize, size_t __lerpTableSize>
        std::array<Sample, __tableSize> LerpTable(std::array<float, __lerpTableSize> rawTable) {
            double scaleFactor = (double)__tableSize / (double)__lerpTableSize;

            std::array<Sample, __tableSize> wavetable;
            for( int i=0; i < __tableSize; i++ )
            {
                double scaledIndex = i/scaleFactor;
                int truncatedIndex = (int)scaledIndex;
                int nextIndex = truncatedIndex + 1;
                double weight =  scaledIndex - truncatedIndex;

                wavetable[i] = std::lerp(
                    rawTable[truncatedIndex],
                    rawTable[nextIndex],
                    weight
                );
            }
            return wavetable;
        }

        // Generate a wavetable for a sine wave.
        template<size_t __tableSize = DEFAULT_WAVETABLE_SIZE>
        std::array<Sample, __tableSize> Sine() {
            std::array<Sample, __tableSize> wavetable;
            for( int i=0; i<__tableSize; i++ )
            {
                wavetable[i] = (float) sin( ((double)i/(double)__tableSize) * M_PI * 2. );
            }

            return wavetable;
        }

        // Generate a wavetable for a pulse wave.
        // pulseWidth 0..1 inclusive
        template<size_t __tableSize = DEFAULT_WAVETABLE_SIZE>
        std::array<Sample, __tableSize> Pulse(float pulseWidth) {
            std::array<Sample, __tableSize> wavetable;
            for( int i=0; i < __tableSize; i++ )
            {
                wavetable[i] = i < ((float)__tableSize * pulseWidth) ? 1. : -1.; 
            }

            return wavetable;
        }

        // Generate a wavetable for a square wave.
        template<size_t __tableSize = DEFAULT_WAVETABLE_SIZE>
        std::array<Sample, __tableSize> Square() {
            return Pulse<__tableSize>(0.5);
        }

        // Generate a wavetable for a saw wave.
        template<size_t __tableSize = DEFAULT_WAVETABLE_SIZE>
        std::array<Sample, __tableSize> Saw() {
            std::array<Sample, __tableSize> wavetable;
            double increment = 2. / (double) __tableSize;
            for( int i=0; i < __tableSize; i++ )
            {
                wavetable[i] = (float) 1. - (i * increment);
            }

            return wavetable;
        }

        // Generate a wavetable for a triangle wave.
        template<size_t __tableSize = DEFAULT_WAVETABLE_SIZE>
        std::array<Sample, __tableSize> Tri() {
            std::array<Sample, 5> lerpTable = {0,1,0,-1,0};
            return LerpTable<__tableSize, 5UL>(lerpTable);
        }
    }
}
