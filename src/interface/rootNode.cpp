
#include "node.h"
#include "rootNode.h"

RootNode::RootNode() : Node(2,2) {}

Frame RootNode::tick(Frame input) 
{
    return input;
}

void RootNode::_tick(int currentTime) {
    Node::_tick(currentTime);
}
