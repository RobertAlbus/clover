#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Delay_Fractional, ShouldDelay) {
  float delayTime = 1.;
  int testSize = 1000;
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collectorDelay(testSize);
  Clover::_Test::Collector<1> collectorIncrementor(testSize);
  Clover::NodeSimplex::Delay::FractionalDelayLine<1> delay(2, delayTime);
  Clover::_Test::Incrementor<1> incrementor(1);

  incrementor >> delay >> collectorDelay >> crank;
  incrementor >> collectorIncrementor >> crank;

  crank.turn(testSize);

  EXPECT_EQ(collectorDelay.frames[0 + delayTime][0],
            collectorIncrementor.frames[0][0]);
  EXPECT_EQ(collectorDelay.frames[1 + delayTime][0],
            collectorIncrementor.frames[1][0]);
  EXPECT_EQ(collectorDelay.frames[2 + delayTime][0],
            collectorIncrementor.frames[2][0]);

  int someLaterFrame = testSize - (2 * delayTime);
  EXPECT_EQ(collectorDelay.frames[someLaterFrame + delayTime][0],
            collectorIncrementor.frames[someLaterFrame][0]);
}

TEST(NodeSimplex_Delay_Fractional, ShouldBeInitialized) {
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(10);
  Clover::NodeSimplex::Delay::FractionalDelayLine<1> delay(6, 2);

  delay >> collector >> crank;

  EXPECT_EQ(delay.settings.initial.delayTime, 2.);
  EXPECT_EQ(delay.settings.initial.delayTime, delay.settings.current.delayTime);

  crank.turn(10);
  EXPECT_EQ(collector.frames[0][0], 0.f);
  EXPECT_EQ(collector.frames[1][0], 0.f);
  EXPECT_EQ(collector.frames[2][0], 0.f);
  EXPECT_EQ(collector.frames[3][0], 0.f);
  EXPECT_EQ(collector.frames[4][0], 0.f);
}