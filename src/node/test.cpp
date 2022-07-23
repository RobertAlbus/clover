// // g++ arity.cpp frame.cpp frameArity.cpp node.cpp nodeArity.cpp test.cpp -o out && ./out && rm ./out

#include <assert.h>
#include <stdio.h>
#include <vector>

#include "arity.h"
#include "node.h"
#include "nodeArity.h"



class Arity1 : public NodeArity<1> {
public:
    Arity1() {

    }
    FrameArity<1> tick(FrameArity<1> input) {
        FrameArity<1> f;

        f.samples[0] = 999.;
        return f;
    }

    /// expose protecteds for testing
    void _tick(int currentTime) {
        NodeArity<1>::_tick(currentTime);
    }
    std::vector<Node*> getInputNodes() {
        return inputNodes;
    }
};

int main() {
    Arity1 a1a;
    Arity1 a1b;

    a1b >> a1a;

    assert(a1a.getArity() == 1);
    assert(a1a.getInputNodes().size() == 1);

    a1a._tick(1);

}