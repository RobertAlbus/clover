#pragma once

#include <cmath>

#include "envelope.h"
#include "subgraph.h"
#include "rootNode.h"

class Adsr : public Subgraph<0,1>
{
public:
    Adsr() : Adsr(0.,0.,1.,0.) { }

    Adsr(size_t a, size_t d, float s, size_t r) :
        Subgraph(), _keyOn(false), envelope(0,0,1)
    {
        connectNodes();
        set(a, d, s, r);
    }

    void set(size_t a, size_t d, float s, size_t r)
    {
        attack(a);
        decay(d);
        sustain(s);
        release(r);
    }

    void connectNodes()
    {
        envelope >> blackHole;
    }

    size_t attack()        { return _attack; }
    void attack(size_t a)  { _attack = a; }

    size_t decay()         { return _decay; }
    void decay(size_t d)   { _decay = d; }

    float sustain()        { return _sustain; }
    void sustain(float s)  { _sustain = s; }

    size_t release()       { return _release; }
    void release(size_t r) { _release =r; }

    void keyOn()  { _keyOn = true;  startTime = _currentClockTime + 1; }
    void keyOff() { _keyOn = false; startTime = _currentClockTime + 1; }
  

protected:
    Envelope envelope;
    size_t _attack;
    size_t _decay;
    float  _sustain;
    size_t _release;
    size_t startTime;
    bool  _keyOn;

    Frame<1> tick(Frame<0> input) override
    {
        // sustain
        // decay
        // attack
        // release
        
        if      ( _keyOn  && _currentClockTime >  startTime + _attack + _decay ) { envelope.set( _sustain,          _sustain,  0);        }
        else if ( _keyOn  && _currentClockTime == startTime + _attack )          { envelope.set( 1.,                _sustain,  _decay);   }
        else if ( _keyOn  && _currentClockTime == startTime )                    { envelope.set( 0.,                1.      ,  _attack);  }
        else if ( !_keyOn && _currentClockTime == startTime )                    { envelope.set( frames.current[0], 0,         _release); }

        return blackHole.frames.current;
    }
};