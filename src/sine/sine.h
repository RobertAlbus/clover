#pragma once

#define TABLE_SIZE   (200)

class Sine : public Node
{
public:
    Sine();

    void freq(float freq);
    float freq();

    // Assign waveform phase with value between 0.0 and 1.0
    void phase(float phase);
    // Get waveform phase as value between 0.0 and 1.0
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

