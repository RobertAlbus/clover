#pragma once

#define TABLE_SIZE   (200)

#ifndef SINE_H
#define SINE_H

class Sine
{
public:
    Sine();

public:
    float tick(const int sample);

    float sine[TABLE_SIZE];
    float phase;
};

#endif
