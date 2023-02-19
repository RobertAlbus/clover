#include <gtest/gtest.h>

#include "Clover.h"

class NodeStub : public Node<1, 1> {
public:
  NodeStub() : Node<1, 1>() {}

  std::vector<Clover::Graph::INode<1> *> &getInputNodes() {
    return Node::inputNodes;
  }

private:
  Frame<1> tick(Frame<1> input) { return Frame<1>{1.f + input[0]}; }
};

TEST(Graph_Node, ShouldConnect) {
  NodeStub stub1;
  NodeStub stub2;
  NodeStub stub3;

  stub2 >> stub1;
  stub3 >> stub1;

  EXPECT_EQ(stub1.getInputNodes().size(), 2);
  EXPECT_EQ(stub1.getInputNodes()[0], &stub2);
  EXPECT_EQ(stub1.getInputNodes()[1], &stub3);

  EXPECT_EXIT(stub2 >> stub1, testing::ExitedWithCode(1), "");
}
