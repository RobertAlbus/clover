#pragma once

#define TABLE_SIZE   (200)

class Sine : public Node
{
public:
    Sine();

    void freq(float freq);
    float freq();

    void phase(float phase);
    float phase();

protected:
    Frame tick(Frame input);

    float lerp();

    Sample wavetable[TABLE_SIZE];
private:
    float _phase;
    float _phaseIncrement;
    float _readonlyFreq;
};

