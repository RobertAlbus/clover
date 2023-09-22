/*
 * /////////
 * // Clover
 *
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 *
 * Copyright (C) 2023 Rob W. Albus
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <gtest/gtest.h>

#include "Graph/AudioNode.h"
#include "Graph/AudioFrame.h"
#include "_Test.h"

class NodeStub : public Clover::Graph::AudioNode<1, 1> {
public:
  NodeStub() : Clover::Graph::AudioNode<1, 1>() {}

  std::vector<Clover::Graph::INode<Clover::Graph::AudioFrame<1>> *> &
  getInputNodes() {
    return Node::inputNodes;
  }

private:
  Clover::Graph::AudioFrame<1> tick(Clover::Graph::AudioFrame<1> input) {
    return Clover::Graph::AudioFrame<1>{1.f + input[0]};
  }
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

  Clover::_Test::HandCrank crank;
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
