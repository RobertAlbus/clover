// g++ test.cpp -o out && ./out && rm ./out 

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <vector>

#include "arity.h"
#include "node.h"

using namespace std;


class Arity1 : public Node {
public:
    Arity1() : Node(1) {

    }
    Frame tick(Frame input) {
        auto f = Frame(arity);

        f.setSampleAtIndex(0, 1. + input.getSampleAtIndex(0));
        return f;
    }

    /// expose protected members for testing
    void _tick(int currentTime) {
        Node::_tick(currentTime);
    }
    int getInputNodesSize() {
        return inputNodes.size();
    }
};

int main() {
    /*
    █───┐
    │   │
    ▼   │
    █   │
    │   │
    ▼   │
    █◀─┘
    │
    ▼
    █
    */
 
    Arity1 root;
    assert(root.getArity() == 1);

    Arity1 a;
    Arity1 b;
    Arity1 c;

    // addresses for debugging
    cout << "root " << &root << endl; 
    cout << "a    " << &a << endl; 
    cout << "b    " << &b << endl; 
    cout << "c    " << &c << endl; 

    c >> b >> a >> root;
    c >> a;

    assert(a.getInputNodesSize() == 2);
    assert(b.getInputNodesSize() == 1);
    assert(c.getInputNodesSize() == 0);
    assert(root.getInputNodesSize() == 1);

    root._tick(0);
    
    assert(c.current().getSampleAtIndex(0) == 1);
    assert(b.current().getSampleAtIndex(0) == 2.);
    assert(a.current().getSampleAtIndex(0) == 4.);
    assert(root.current().getSampleAtIndex(0) == 5.);

    root._tick(1);

    assert(c.current().getSampleAtIndex(0) == 1);
    assert(b.current().getSampleAtIndex(0) == 2.);
    assert(a.current().getSampleAtIndex(0) == 4.);
    assert(root.current().getSampleAtIndex(0) == 5.);

}