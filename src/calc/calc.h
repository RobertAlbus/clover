#pragma once

#include <cmath>

namespace calc 
{

    float ltodb(float x)
    {
        return log10(fabs(x)) * 20.;
    }

    float dbtol(float x)
    {
        return powf32(10., x / 20.);
    }

    float mtof(float x)
    {
        return 440. * powf32(2., (x-69.)/12.);
    }

    float ftom(float x)
    {
        return 12. * ( log(x/220.)/log(2) ) + 57.;
    }
};