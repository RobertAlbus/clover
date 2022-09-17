#pragma once

#include <cmath>
#include <algorithm>

#include "constants.h"
#include "firLowPass.h"
#include "../nodeSimplex.h"

namespace SVFd
{
  constexpr int lowpass  = 1;
  constexpr int bandpass = 2;
  constexpr int highpass = 3;

  constexpr int lp = lowpass;
  constexpr int bp = bandpass;
  constexpr int hp = highpass;
}

class SVF : public Subgraph<1,1>
{
public:
  SVF(float __cutoff, float reso, int mode = 1, int oversamplingFactor = 4, float __sampleRate = SAMPLE_RATE) : Subgraph(),
    dt_prime(0), fb_prime(0), _mode(mode), _oversamplingFactor(oversamplingFactor),
    lp(0), bp(0), hp(0)
  {
    cutoff(__cutoff);
    q(reso);
    fir >> blackHole;
  }

  void  cutoff(float freq) { _cutoff = freq; }
  float cutoff()           { return _cutoff; }

  void  q(float reso) { _q = reso; }
  float q()           { return _q; }

  void mode(int type) { _mode = type; } // TODO: value-check
  int  mode()         { return _mode; }

protected:
  FIRLP fir;

  float _cutoff;
  float _q;
  int _mode;
  int _oversamplingFactor;

  float dt_prime;
  float fb_prime;

  Sample lp;
  Sample bp;
  Sample hp;

  void oversamplingFactor(int over) { _oversamplingFactor = over; } // TODO: update FIR
  int  oversamplingFactor()         { return _oversamplingFactor; }

  Frame<1> tick(Frame<1> inputFrame)
  {
    // noise term
    float noise = ((float)rand()) / (float)(RAND_MAX);
    noise = 2.0 * (noise - 0.5);

    // integration rate
    float dt = 0.001+2.25*(_cutoff*_cutoff*_cutoff*_cutoff);

    float feedback = 1.0 - _q;

    // smooth parameter changes
    // dt_prime = (dt * (1.0-0.999)) + (dt_prime * 0.999); 
    // fb_prime = (feedback * (1.0-0.993)) + (fb_prime * 0.993); 

    dt_prime = dt; 
    fb_prime = feedback; 

    // integrate filter state with oversampling
    for(int nn = 0; nn < _oversamplingFactor; nn++){
      hp = inputFrame[0] - (2.0f*fb_prime-1.0f)*bp - lp  + 1.0e-6*noise;
      bp += (dt_prime / _oversamplingFactor) * hp;
      bp = std::tanh(bp);
      lp += (dt_prime / _oversamplingFactor) * bp;  
      lp = std::tanh(lp);
    }

    float out = 0.;

    switch (_mode)
    {
      case 1:
        out = lp;
        break;
      case 2:
        out = bp;
        break;
      case 3:
        out = hp;
        break;
      default:
        out = lp;
    }

    // fir.frames.next(Frame<1>{out});
    // return fir.frames.current;
    return Frame<1>{out};
  }
};
