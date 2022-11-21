#pragma once

#include <cmath>

#include "NodeSimplex.h"
#include "graph.h"


namespace Clover::NodeSimplex::Envelope {

struct AdsrSettings
{
  AdsrSettings() : AdsrSettings(0.,0.,1.,0.) { }
  AdsrSettings(size_t a, size_t d, float s, size_t r)
  : attack(a), decay(d), sustain(s), release(r),
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
    connectNodes();
    set(
      settings.initial.attack,
      settings.initial.decay,
      settings.initial.sustain,
      settings.initial.attack
    );
  }

  void set(size_t a, size_t d, float s, size_t r)
  {
    attack(a);
    decay(d);
    sustain(s);
    release(r);
  }

  size_t attack()        { return settings.current.attack; }
  void attack(size_t a)  { settings.current.attack = a; }

  size_t decay()         { return settings.current.decay; }
  void decay(size_t d)   { settings.current.decay = d; }

  float sustain()        { return settings.current.sustain; }
  void sustain(float s)  { settings.current.sustain = s; }

  size_t release()       { return settings.current.release; }
  void release(size_t r) { settings.current.release = r; }

  void keyOn()  { settings.current.keyOn = true;  settings.current.startTime = _currentClockTime + 1; }
  void keyOff() { settings.current.keyOn = false; settings.current.startTime = _currentClockTime + 1; }

protected:
  BasicEnvelope envelope;

  Frame<1> tick(Frame<0> input)
  {
    AdsrSettings& s = settings.current;

    bool isStartOfSustain =  s.keyOn && _currentClockTime >  s.startTime + s.attack + s.decay;
    bool isStartOfDecay   =  s.keyOn && _currentClockTime == s.startTime + s.attack;
    bool isStartOfAttack  =  s.keyOn && _currentClockTime == s.startTime;
    bool isStartOfRelease = !s.keyOn && _currentClockTime == s.startTime;
    
    if      ( isStartOfSustain ) { envelope.set( s.sustain,         s.sustain,  0        ); }
    else if ( isStartOfDecay   ) { envelope.set( 1.,                s.sustain,  s.decay  ); }
    else if ( isStartOfAttack  ) { envelope.set( 0.,                1.,         s.attack ); }
    else if ( isStartOfRelease ) { envelope.set( frames.current[0], 0,          s.release); }

    return blackHole.frames.current;
  }

  void connectNodes()
  {
    envelope >> blackHole;
  }
};

}