#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_Delay_Fractional, SmokeTest) {
  float delayTime = 1.f;
  int testSize = delayTime + 10;
  Clover::_Test::HandCrank<1> crank;
  Clover::NodeSimplex::Delay::FractionalDelayLine<1> delay(testSize, delayTime);
  Clover::_Test::Collector<1> delayCollector(testSize);
  Clover::_Test::Incrementor<1> incrementor(1);
  Clover::_Test::Collector<1> incrementorCollector(testSize);

  incrementor >> delay >> delayCollector >> crank;
  incrementor >> incrementorCollector >> crank;

  crank.turn(testSize);

  for (int i = 0; i < testSize - delayTime; i++) {
    EXPECT_EQ(delayCollector.frames[i + delayTime][0],
              incrementorCollector.frames[i][0])
        << "If this test has failed, it's likely the "
           "Delay::FractionalDelay algorithm.";
  }
}
