#pragma once

#define TABLE_SIZE   (200)

class Sine : public Node
{
public:
    Sine();

    /// Set frequency
    void freq(float freq);

    /// Get frequency
    float freq();

    /// set phase
    void phase(float phase);
    
    // Get phase
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

