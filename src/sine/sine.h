#pragma once

#define TABLE_SIZE   (200)

class Sine
{
public:
    Sine();
    float tick(const int sample);

    float sine[TABLE_SIZE];
    float phase;
};

