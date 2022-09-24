#pragma once

#include <math.h>

#include "constants.h"
#include "node.h"

namespace Clover::NodeSimplex::Filter {

template <int __order = 128, size_t __sampleRate = SAMPLE_RATE>
class FirLowPass : public Node<1,1>
{
public:
  FirLowPass(float cut = 20000.) : _cutoff(cut), _sampleRate(__sampleRate), _order((int)__order)
  {
    computeImpulseResponse();
  }

protected:
  // filter design variables
  float _sampleRate;
  float _cutoff;
  int   _order;

  // dsp variables
  float omega_c;
  float h_d[__order];
  float h[__order];
  float w[__order];

  Sample ringBuffer[__order];
  int ringBufferIndex;

  void computeImpulseResponse(){

    // index as -M..M
    float ii;
    
    // set cutoff frequency in radians
    omega_c = (_cutoff/_sampleRate) * 2.0 * (float)M_PI;

    // compute truncated ideal impulse response
    for(size_t n=0; n < _order; n++){
      // compute index as -M..M and avoid NaN at impulse peak
      ii = (float)(n) - 1.0 - floor((float)_order/2.0f) + 1.0e-9;

      // sample sinc function
      h_d[n] = omega_c * std::sin((omega_c * ii))/(omega_c * ii);
    }

    // compute windowing function
    for(int n=0; n < _order; n++){
      // compute index as -M..M and avoid NaN at impulse peak
      ii = (float)n - 1.0 - floor(_order / 2.0) + 1.0e-9;

      // hanning window function
      w[n] = std::cos((float)M_PI * ii/_order);
      w[n] *= w[n];
    }

    // compute windowed ideal impulse function
    for(int n=0; n < _order; n++){
      // window truncated ideal impulse response
      h[n] = w[n] * h_d[n];
    }
  }

  Frame<1> tick(Frame<1> inputFrame)
  {
    ringBuffer[ringBufferIndex++] = inputFrame[0];
    if(ringBufferIndex > (_order - 1)){
      ringBufferIndex -= _order;
    }

    // compute convolution
    Sample output = 0.0;
    int ii;
    for(int n = 0; n < _order; n++){
      // wrap around index
      ii = (ringBufferIndex - (n + 1));
      if(ii < 0)
        ii += _order;
      
      // multiply and accumulate
      output += h[n] * ringBuffer[ii]; 
    }

    return Frame<1> {output};
  }

};

class FIRLP : public FirLowPass<39, SAMPLE_RATE> {};

}