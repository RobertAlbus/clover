#pragma once

#include <algorithm>
#include <cmath>

#include "NodeSimplex.h"
#include "Graph.h"


namespace Clover::NodeSimplex::Envelope {

struct AdsrSettings
{
  AdsrSettings() : AdsrSettings(0.,0.,1.,0.) { }
  AdsrSettings(size_t a, size_t d, float s, size_t r) :
    attack (std::max((size_t)1,a)),
    decay  (std::max((size_t)1,d)),
    sustain(s),
    release(std::max((size_t)1,r)),
    startTime(0), keyOn(false) {}

  size_t attack;
  size_t decay;
  float  sustain;
  size_t release;
  size_t startTime;
  bool   keyOn;
};

class Adsr : public StatefulSubgraph<0,1, AdsrSettings>
{
public:
  Adsr(AdsrSettings initialSettings) : StatefulSubgraph(initialSettings), envelope(0,0,0)
  {
    settings.reset();
    connectNodes();
  }

  void set(size_t a, size_t d, float s, size_t r)
  {
    attack(a);
    decay(d);
    sustain(s);
    release(r);
  }

  size_t attack()        { return settings.current.attack; }
  void attack(size_t a)  { settings.current.attack = std::max((size_t)1,a); }

  size_t decay()         { return settings.current.decay; }
  void decay(size_t d)   { settings.current.decay = std::max((size_t)1,d); }

  float sustain()        { return settings.current.sustain; }
  void sustain(float s)  { settings.current.sustain = s; }

  size_t release()       { return settings.current.release; }
  void release(size_t r) { settings.current.release = std::max((size_t)1,r); }

  void keyOn()
  {
    AdsrSettings& s = settings.current;
    envelope.set( 0.f, 1.f, s.attack); 

    settings.current.keyOn = true;
    settings.current.startTime = _currentClockTime;
  }
  void keyOff()
  {
    AdsrSettings& s = settings.current;

    envelope.set( frames.current[0], 0., s.release );

    settings.current.keyOn = false;
    settings.current.startTime = _currentClockTime;
  }

protected:
  BasicEnvelope envelope;

  Frame<1> tick(Frame<0> input)
  {
    AdsrSettings& s = settings.current;

    // should compute these times and store them in settings
    bool isStartOfDecay   =  s.keyOn && _currentClockTime == s.startTime + s.attack;
    bool isStartOfSustain =  s.keyOn && _currentClockTime == s.startTime + s.attack + s.decay;

      
    if ( isStartOfSustain ) 
    {
      envelope.set( s.sustain, s.sustain, 0); 
    }
    else if ( isStartOfDecay )
    {
      envelope.set( 1., s.sustain, s.decay);
    }

    return blackHole.frames.current;
  }

  void connectNodes()
  {
    envelope >> blackHole;
  }
};

}