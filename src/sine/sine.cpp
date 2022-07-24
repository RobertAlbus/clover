#include <stdio.h>
#include <math.h>
#include "constants.h"
#include "node.h"
#include "sine.h"

#define TABLE_SIZE   (200)

Sine::Sine() : Node(0,2), phase(0)
{
    /* initialise sinusoidal wavetable */
    for( int i=0; i<TABLE_SIZE; i++ )
    {
        sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
}

Frame Sine::tick(Frame input)
{
    Sample value = sine[(int)phase++];

    if (phase >= TABLE_SIZE) {
        phase -= TABLE_SIZE;
    }

    Frame f = Frame(2);
    f.setSampleAtIndex(0, value);
    f.setSampleAtIndex(1, value);

    return f;
}
