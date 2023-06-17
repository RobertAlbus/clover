#include <gtest/gtest.h>

#include "Clover.h"

class NodeStub : public AudioNode<1, 1> {
public:
  NodeStub() : AudioNode<1, 1>() {}

  std::vector<Clover::Graph::INode<Frame<1>> *> &getInputNodes() {
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

TEST(Graph_Node, ShouldSumAndGain) {

  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(1);

  NodeStub stub1;
  NodeStub stub2;
  NodeStub stub3;

  stub1 >> collector >> crank;

  stub2 >> stub1;
  stub3 >> stub1;

  crank.turn(1);
  stub1.gain(2);
  crank.turn(1);

  EXPECT_EQ(collector.frames[0][0], 3.f);
  EXPECT_EQ(collector.frames[1][0], 6.f);
}
