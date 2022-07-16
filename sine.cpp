#include <stdio.h>
#include <math.h>


#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)

class Sine
{
public:
    Sine() : phase(0)
    {
        /* initialise sinusoidal wavetable */
        for( int i=0; i<TABLE_SIZE; i++ )
        {
            sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
        }
    }

public:
    float tick(const int sample)
    {
        float value = sine[(int)phase];
        phase += 1;
        if( phase >= TABLE_SIZE ) {
            phase -= TABLE_SIZE;
        }
    
        return value;
    }

    float sine[TABLE_SIZE];
    float phase;
};

