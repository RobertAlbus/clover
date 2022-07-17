#include <stdio.h>
#include <math.h>
#include "sine.h"

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)


Sine::Sine() : phase(0)
{
    /* initialise sinusoidal wavetable */
    for( int i=0; i<TABLE_SIZE; i++ )
    {
        sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
}

float Sine::tick(const int sample)
{
    float value = sine[(int)phase++];
    if( phase >= TABLE_SIZE ) {
        phase -= TABLE_SIZE;
    }

    return value;
}
