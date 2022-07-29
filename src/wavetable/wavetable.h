#include <array>
#include <cmath>
#include <math.h>

namespace Wavetable {
    namespace Generate {
        constexpr int default_table_size = 200;
        // Generate a wavetable for a sine wave.

        template <int __tableSize = default_table_size>
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
        template <int __tableSize = default_table_size>
        std::array<Sample, __tableSize> Pulse(float pulseWidth) {
            std::array<Sample, __tableSize> wavetable;
            for( int i=0; i < __tableSize; i++ )
            {
                wavetable[i] = i < ((float)__tableSize * pulseWidth) ? 1. : -1.; 
            }

            return wavetable;
        }

        // Generate a wavetable for a square wave.
        template <int __tableSize = default_table_size>
        std::array<Sample, __tableSize> Square(int tableSize) {
            return Pulse<__tableSize>(0.5);
        }

        // Generate a wavetable for a saw wave.
        template <int __tableSize = default_table_size>
        std::array<Sample, __tableSize> Saw() {
            std::array<Sample, __tableSize> wavetable;
            double increment = 2. / (double) __tableSize;
            for( int i=0; i < __tableSize; i++ )
            {
                wavetable[i] = (float) 2. - (i * increment);
            }

            return wavetable;
        }

        // Generate a wavetable for a triangle wave.
        template <int __tableSize = default_table_size>
        std::array<Sample, __tableSize> Tri(int tableSize) {
            std::array<Sample, 5> lerpTable = {0,1,0,-1,0};
            return LerpTable<__tableSize, 5>(tableSize, lerpTable);
        }


        template <int __tableSize = default_table_size, int __lerpTableSize>
        std::array<Sample, __tableSize> LerpTable(int tableSize, std::array<float, __lerpTableSize> rawTable) {
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
    }
}
