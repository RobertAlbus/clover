#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Constants.h"
#include "Graph.h"


namespace Clover::NodeSimplex::Filter {


const float RECIP_SQRT_2 = static_cast<float>( M_SQRT1_2 );  
const float TWO_PI = M_PI * 2.0f;

struct BiQuadSettings
{
  void (*setFunction)(float, float);
  float freq;
  float Q;

  float K_;
  float kSqr_;
  float denom_;
  std::array<float, 3> b_;
  std::array<float, 3> a_;

};

template <size_t __arity>
class BiQuad : public Node<__arity, __arity>
{
public:

  //! Default constructor creates a second-order pass-through filter.
  BiQuad() : inputs_{}, outputs_{}
  {
    b_[0] = 1.0f;
    a_[0] = 1.0f;

    K_ = 0.0f;
    kSqr_ = 0.0f;
    denom_ = 1.0f;
  }

  //! Set all filter coefficients.
  void setCoefficients( float b0, float b1, float b2, float a1, float a2, bool clearState )
  {
    b_[0] = b0;
    b_[1] = b1;
    b_[2] = b2;
    a_[1] = a1;
    a_[2] = a2;

    if ( clearState ) this->clear();
  }

  //! Set the b[0] coefficient value.
  void setB0( float b0 ) { b_[0] = b0; };

  //! Set the b[1] coefficient value.
  void setB1( float b1 ) { b_[1] = b1; };

  //! Set the b[2] coefficient value.
  void setB2( float b2 ) { b_[2] = b2; };

  //! Set the a[1] coefficient value.
  void setA1( float a1 ) { a_[1] = a1; };

  //! Set the a[2] coefficient value.
  void setA2( float a2 ) { a_[2] = a2; };

  std::function<void(float, float)> setFunction;

  void set(float f, float q)
  {
    setFunction(f, q);
  }

  void freq(float frequency)
  {
    setFunction(frequency, reso_);
  }

  void reso(float Q)
  {
    setFunction(freq_, Q);
  }

  void lowPass()
  {
    setFunction = [this](float freq, float reso) {
      this->setLowPass(freq, reso);
    };
  }
  
  void highPass()
  {
    setFunction = [this](float freq, float reso) {
      this->setHighPass(freq, reso);
    };
  }
  
  void allPass()
  {
    setFunction = [this](float freq, float reso) {
      this->setAllPass(freq, reso);
    };
  }

  void bandReject()
  {
    setFunction = [this](float freq, float reso) {
      this->setBandReject(freq, reso);
    };
  }

  void bandPass()
  {
    setFunction = [this](float freq, float reso) {
      this->setBandPass(freq, reso);
    };
  }

  Frame<__arity> tick(Frame<__arity> input)
  {
    inputs_[0] = input;

    for (int i = 0; i < __arity; i++)
    {
      lastFrame_[i]  = b_[0] * inputs_[0][i]  + b_[1] * inputs_[1][i] + b_[2] * inputs_[2][i];
      lastFrame_[i] -= a_[2] * outputs_[2][i] + a_[1] * outputs_[1][i];
    }

    inputs_[2] = inputs_[1];
    inputs_[1] = inputs_[0];
    outputs_[2] = outputs_[1];
    outputs_[1] = lastFrame_;

    return lastFrame_;
  }


  //! Sets the filter coefficients for a resonance at \e frequency (in Hz).
  /*!
    This method determines the filter coefficients corresponding to
    two complex-conjugate poles with the given \e frequency (in Hz)
    and \e radius from the z-plane origin.  If \e normalize is true,
    the filter zeros are placed at z = 1, z = -1, and the coefficients
    are then normalized to produce a constant unity peak gain
    (independent of the filter \e gain parameter).  The resulting
    filter frequency response has a resonance at the given \e
    frequency.  The closer the poles are to the unit-circle (\e radius
    close to one), the narrower the resulting resonance width.
    An unstable filter will result for \e radius >= 1.0.  The
    \e frequency value should be between zero and half the sample rate.
  */
  void setResonance( float frequency, float radius, bool normalize )
  {
    a_[2] = radius * radius;
    a_[1] = -2.0 * radius * cos( TWO_PI * frequency / (float)SAMPLE_RATE );

    if ( normalize ) {
      // Use zeros at +- 1 and normalize the filter peak gain.
      b_[0] = 0.5 - 0.5 * a_[2];
      b_[1] = 0.0;
      b_[2] = -b_[0];
    }
    else {
      b_[0] = 1.0;
      b_[1] = 0.0;
      b_[2] = 0.0;
    }
  }
  //! Set the filter coefficients for a notch at \e frequency (in Hz).
  /*!
    This method determines the filter coefficients corresponding to
    two complex-conjugate zeros with the given \e frequency (in Hz)
    and \e radius from the z-plane origin.  No filter normalization is
    attempted.  The \e frequency value should be between zero and half
    the sample rate.  The \e radius value should be positive.
  */
  void setNotch( float frequency, float radius )
  {
    // This method does not attempt to normalize the filter gain.
    b_[0] = 1.0; 
    b_[1] = (float) -2.0 * radius * cos( TWO_PI * frequency / (float)SAMPLE_RATE );
    b_[2] = radius * radius;
    
    a_[1] = 0.0; 
    a_[2] = 0.0;
  }

  //! Set the filter coefficients for a low-pass with cutoff frequency \e fc (in Hz) and Q-factor \e Q.
  /*!
    This method determines the filter coefficients corresponding to a 
    low-pass filter with cutoff placed at \e fc, where sloping behaviour 
    and resonance are determined by \e Q. The default value for \e Q is 
    1/sqrt(2), resulting in a gradual attenuation of frequencies higher than
    \e fc without added resonance. Values greater than this will more 
    aggressively attenuate frequencies above \e fc while also adding a 
    resonance at \e fc. Values less than this will result in a more gradual
    attenuation of frequencies above \e fc, but will also attenuate 
    frequencies below \e fc as well. Both \e fc and \e Q must be positive.
  */
  void setLowPass( float fc, float Q )
  {
    setCommonFilterValues(fc, Q);

    b_[0] = kSqr_ * Q * denom_;
    b_[1] = 2 * b_[0];
    b_[2] = b_[0];
  }

  //! Set the filter coefficients for a high-pass with cutoff frequency \e fc (in Hz) and Q-factor \e Q.
  /*!
    This method determines the filter coefficients corresponding to a high-pass 
    filter with cutoff placed at \e fc, where sloping behaviour and resonance 
    are determined by \e Q. The default value for \e Q is 1/sqrt(2), resulting 
    in a gradual attenuation of frequencies lower than \e fc without added 
    resonance. Values greater than this will more aggressively attenuate 
    frequencies below \e fc while also adding a resonance at \e fc. Values less 
    than this will result in a more gradual attenuation of frequencies below 
    \e fc, but will also attenuate frequencies above \e fc as well. 
    Both \e fc and \e Q must be positive.
  */
  void setHighPass( float fc, float Q=RECIP_SQRT_2 )
  {
    setCommonFilterValues(fc, Q);

    b_[0] = Q * denom_;
    b_[1] = -2 * b_[0];
    b_[2] = b_[0];
  }

  //! Set the filter coefficients for a band-pass centered at \e fc (in Hz) with Q-factor \e Q.
  /*!
    This method determines the filter coefficients corresponding to a band-pass
    filter with pass-band centered at \e fc, where band width and slope a 
    determined by \e Q. Values for \e Q that are less than 1.0 will attenuate
    frequencies above and below \e fc more gradually, resulting in a convex 
    slope and a wider band. Values for \e Q greater than 1.0 will attenuate 
    frequencies above and below \e fc more aggressively, resulting in a 
    concave slope and a narrower band. Both \e fc and \e Q must be positive.
  */
  void setBandPass( float fc, float Q )
  {
    setCommonFilterValues(fc, Q);

    b_[0] = K_ * denom_;
    b_[1] = 0.0;
    b_[2] = -b_[0];
  }

  //! Set the filter coefficients for a band-reject centered at \e fc (in Hz) with Q-factor \e Q.
  /*!
    This method determines the filter coefficients corresponding to a 
    band-reject filter with stop-band centered at \e fc, where band width 
    and slope are determined by \e Q. Values for \e Q that are less than 1.0 
    will yield a wider band with greater attenuation of \e fc. Values for \e Q
    greater than 1.0 will yield a narrower band with less attenuation of \e fc. 
    Both \e fc and \e Q must be positive.
  */
  void setBandReject( float fc, float Q )
  {
    setCommonFilterValues(fc, Q);

    b_[0] = Q * (kSqr_ + 1) * denom_;
    b_[1] = 2 * Q * (kSqr_ - 1) * denom_;
    b_[2] = b_[0];
  }

  //! Set the filter coefficients for an all-pass centered at \e fc (in Hz) with Q-factor \e Q.
  /*!
    This method determines the filter coefficients corresponding to
    an all-pass filter whose phase response crosses -pi radians at \e fc.
    High values for \e Q will result in a more instantaenous shift in phase 
    response at \e fc. Lower values will result in a more gradual shift in
    phase response around \e fc. Both \e fc and \e Q must be positive.
  */
  void setAllPass( float fc, float Q )
  {
    setCommonFilterValues(fc, Q);

    b_[0] = a_[2];
    b_[1] = a_[1];
    b_[2] = 1;
  }

  //! Sets the filter zeroes for equal resonance gain.
  /*!
    When using the filter as a resonator, zeroes places at z = 1, z
    = -1 will result in a constant gain at resonance of 1 / (1 - R),
    where R is the pole radius setting.
  */
  void setEqualGainZeroes()
  {
    b_[0] = 1.0;
    b_[1] = 0.0;
    b_[2] = -1.0;
  }

protected:

  float freq_;
  float reso_;

  float K_;
  float kSqr_;
  float denom_;

  Frame<__arity> lastFrame_;

  std::array<float, 3> b_;
  std::array<float, 3> a_;
  FrameBuffer<__arity, 3> outputs_;
  FrameBuffer<__arity, 3> inputs_;

  // Helper function to update the three intermediate values for the predefined filter types
  // along with the feedback filter coefficients. Performs the debug check for fc and Q-factor arguments.
  void setCommonFilterValues( float fc, float Q)
  {
    K_ = tan(PI * fc / (double)SAMPLE_RATE);
    kSqr_ = K_ * K_;
    denom_ = 1 / (kSqr_ * Q + K_ + Q);

    a_[1] = 2 * Q * (kSqr_ - 1) * denom_;
    a_[2] = (kSqr_ * Q - K_ + Q) * denom_;
  }

  void clear()
  {
    for ( int i = 0; i < 1; i++ )
    {
      inputs_[i] = 0.0;
      outputs_[i] = 0.0;
    }
    lastFrame_ = 0.0;
  }

};

}
