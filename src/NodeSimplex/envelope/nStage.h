#pragma once

#include <vector>

#include "node.h"

using namespace Clover::Graph;

namespace Clover::NodeSimplex::Envelope {

template <size_t __stages, size_t __arity>
class NStage : public Node<__arity,__arity>
{
public:
  NStage() : Node<__arity,__arity>() {}

  struct Stage {
    Stage()
    bool hasCurrent;
    float current;
    float target;
    int duration;
  };

  void set(std::vector<Stage> stages)
  {
    _stages
  }

  void keyOn() {}
  void keyOff() {}
private:
  std::vector<Stage> _stages
  Frame<__arity> tick(Frame<__arity> input)
  {
    Frame<__arity> f;
    for (int i = 0; i < __arity; i++)
    {
      f[i] = 0;
    }
    return f;
  }
};



}