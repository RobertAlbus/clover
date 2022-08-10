#pragma once

#include <cmath>

#include "difference.h"
#include "node.h"
#include "pan.h"
#include "rootNode.h"
#include "sum.h"

class Stereo : public Node<2,2>
{
public:
  Stereo() : Node(), pan2(0)
  {
    connectNodes();
    spread(0);
  }

  Stereo(float s, float p) : pan2(p)
  {
    connectNodes();
    spread(s);
  }

  void connectNodes()
  {
    spring >> mid  >> pan2;
    spring >> side >> pan2;
    pan2 >> blackHole;
  }

  void spread(float s)
  {
    _spreadReadOnly = s;

    s = fabs(_spreadReadOnly);
    float neutralGain = dbtol(-4.5);
    float gainUp  = lerp(neutralGain, 0., s);
    float gainDown = lerp(neutralGain, 1., s);

    mid.gain  = _spreadReadOnly < 0 ? gainUp   : gainDown;
    side.gain = _spreadReadOnly < 0 ? gainDown : gainUp;
  }

  float spread()
  {
    return _spreadReadOnly;
  }

  void pan(float p)
  {
    pan2.pan(p);
  }
  

protected:
  Sum2 mid;
  Difference side;
  Pan2 pan2;
  RootNode<2> spring;
  RootNode<2> blackHole;

  float _spreadReadOnly;
  
  void _tickCallback(int currentClockTime) override
  {
    blackHole._tick(currentClockTime);
  }

  Frame<2> tick(Frame<2> input) override
  {
    spring.frames.next(input);
    return blackHole.frames.current;
  }
};