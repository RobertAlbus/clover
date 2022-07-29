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

    std::array<Sample, TABLE_SIZE> wavetable;
private:
    float _phase;
    float _phaseIncrement;
    float _readonlyFreq;
};

