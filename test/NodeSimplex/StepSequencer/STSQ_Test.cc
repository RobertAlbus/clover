#include <vector>

#include <gtest/gtest.h>

#include "Clover.h"

struct TestTarget {
  TestTarget() : accumulator(0) {}
  void increment() {
    accumulator++;
    printf("\n accumulated: %i", accumulator);
  }
  int accumulator;
};

void TestFacilitator(const int &data, std::vector<TestTarget *> &targets) {
  for (int i = 0; i < targets.size(); i++) {
    targets[i]->increment();
  }
}

TEST(NodeSimplex_StepSequencer, ShouldStepCorrectly) {

  Clover::_Test::HandCrank<0> crank;
  STSQ<int, TestTarget, TestFacilitator> stepSequencer;
  stepSequencer >> crank;

  TestTarget target;
  stepSequencer.targets.emplace_back(&target);

  std::vector<int> stepData{1, 1, 1, 1};
  STSQ_Pattern<int> pattern;
  pattern.from(stepData, 10.2);
  stepSequencer.addPattern(pattern);

  EXPECT_EQ(target.accumulator, 0);
  crank.turn(1); // first step
  EXPECT_EQ(target.accumulator, 1);
  crank.turn(10);
  EXPECT_EQ(target.accumulator, 2);
  crank.turn(10);
  EXPECT_EQ(target.accumulator, 3);
  crank.turn(10);
  EXPECT_EQ(target.accumulator, 3);
  crank.turn(1); // 0.2 remainder is accounted for
  EXPECT_EQ(target.accumulator, 4);
}

TEST(NodeSimplex_StepSequencer, ShouldBeAbleToChangePattern) {

  Clover::_Test::HandCrank<0> crank;
  STSQ<int, TestTarget, TestFacilitator> stepSequencer;
  stepSequencer >> crank;

  TestTarget target;
  stepSequencer.targets.emplace_back(&target);

  std::vector<int> stepData1{1, 1, 1, 1};
  STSQ_Pattern<int> pattern1;
  STSQ_Pattern<int> pattern2;

  pattern1.from(stepData1, 1);
  pattern2.from(stepData1, 2);
  stepSequencer.addPattern(pattern1);
  stepSequencer.addPattern(pattern2);

  stepSequencer.setPattern(0);
  EXPECT_EQ(target.accumulator, 0);
  crank.turn(4);
  EXPECT_EQ(target.accumulator, 4);

  stepSequencer.setPattern(1);
  crank.turn(4);
  EXPECT_EQ(target.accumulator, 6);
  crank.turn(5);
  EXPECT_EQ(target.accumulator, 9);

  stepSequencer.setPattern(0);
  EXPECT_EQ(target.accumulator, 10);
}
