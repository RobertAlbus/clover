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

#include "Clover/_Test/Collector.h"
#include "Clover/_Test/DCN.h"
#include "Clover/_Test/HandCrank.h"
#include "Clover/_Test/Incrementor.h"

#include "Clover/Nodes/Adapter/NullAdapter.h"
#include "Clover/Nodes/Delay/FractionalDelayLine.h"
#include "Clover/Nodes/Envelope/ADSR.h"
#include "Clover/Nodes/Envelope/BasicEnvelope.h"
#include "Clover/Nodes/Stereo/Difference.h"
#include "Clover/Nodes/Stereo/MidSideBalance.h"
#include "Clover/Nodes/Stereo/Pan.h"
#include "Clover/Nodes/Stereo/Sum.h"
#include "Clover/Nodes/Wavetable/WavetableOsc.h"
#include "Clover/Nodes/Wavetable/WavetableOscStereo.h"

TEST(Nodes_SmokeTest, Delay_Fractional) {
  float delayTime = 1.f;
  int testSize = delayTime + 10;
  Clover::_Test::HandCrank crank;
  Clover::Nodes::Delay::FractionalDelayLine<1> delay(testSize, delayTime);
  Clover::_Test::Collector<1> delayCollector(testSize);
  Clover::_Test::Incrementor<1> incrementor(1);
  Clover::_Test::Collector<1> incrementorCollector(testSize);

  incrementor >> delay >> delayCollector >> crank;
  incrementor >> incrementorCollector >> crank;

  crank.turn(testSize);

  for (int i = 0; i < testSize - delayTime; i++) {
    EXPECT_EQ(
        delayCollector.frames[i + delayTime][0],
        incrementorCollector.frames[i][0]
    ) << "If this test has failed, it's likely the "
         "Delay::FractionalDelay algorithm.";
  }
}

TEST(Nodes_SmokeTest, Envelope_Basic) {
  Clover::_Test::HandCrank crank;
  Clover::Nodes::Envelope::BasicEnvelope envelope;
  Clover::_Test::Collector<1> envelopeCollector(100);

  envelope >> envelopeCollector >> crank;

  crank.turn(50);
  envelope.set(1.f, 40);
  crank.turn(50);

  for (int i = 0; i < 50; i++) {
    EXPECT_EQ(envelopeCollector.frames[i][0], 0);
  }

  for (int i = 0; i < 40; i++) {
    EXPECT_GE(envelopeCollector.frames[i + 50][0], 0.f);
    EXPECT_LE(envelopeCollector.frames[i + 50][0], 1.f);
  }
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(envelopeCollector.frames[i + 90][0], 1.f);
  }
}

TEST(Nodes_SmokeTest, Envelope_ADSR) {
  Clover::_Test::HandCrank crank;
  Clover::Nodes::Envelope::ADSR envelope;
  Clover::_Test::Collector<1> envelopeCollector(100);

  envelope >> envelopeCollector >> crank;

  crank.turn(10);
  envelope.set(10, 10, 0.5, 10);
  envelope.triggerOn();
  crank.turn(25);
  envelope.triggerOff();
  crank.turn(20);

  EXPECT_EQ(envelopeCollector.frames[0][0], 0.f);
  EXPECT_EQ(envelopeCollector.frames[9][0], 0.f);
  EXPECT_EQ(envelopeCollector.frames[19][0], 1.f);
  EXPECT_EQ(envelopeCollector.frames[29][0], 0.5f);
  EXPECT_EQ(envelopeCollector.frames[34][0], 0.5f);
  EXPECT_EQ(envelopeCollector.frames[44][0], 0.f);
  EXPECT_EQ(envelopeCollector.frames[54][0], 0.f);
}

TEST(Nodes_SmokeTest, Stereo_Difference) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<2> collector(2);
  Clover::Nodes::Stereo::Difference difference;
  Clover::_Test::DCN<2> dc;

  dc >> difference >> collector >> crank;

  dc.indexBasis(0); // [0,1]
  crank.turn(1);

  EXPECT_FLOAT_EQ(collector.frames[0][0], -0.5f);
  EXPECT_FLOAT_EQ(collector.frames[0][1], 0.5f);
}

TEST(Nodes_SmokeTest, Stereo_MidSideBalance) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<2> collector(3);
  Clover::Nodes::Stereo::MidSideBalance ms;
  Clover::_Test::DCN<2> dc;

  dc >> ms >> collector >> crank;

  dc.indexBasis(0); // [0,1]
  ms.spread(0);
  crank.turn(1);
  ms.spread(1);
  crank.turn(1);
  ms.spread(-1);
  crank.turn(1);

  EXPECT_FLOAT_EQ(collector.frames[0][0], 0.f);
  EXPECT_FLOAT_EQ(collector.frames[0][1], 1.f);
  EXPECT_FLOAT_EQ(collector.frames[1][0], -0.5f);
  EXPECT_FLOAT_EQ(collector.frames[1][1], 0.5f);
  EXPECT_FLOAT_EQ(collector.frames[2][0], 0.5f);
  EXPECT_FLOAT_EQ(collector.frames[2][1], 0.5f);
}

TEST(Nodes_SmokeTest, Stereo_Pan1) {
  std::vector<float> wtRaw = {1.f, 1.f, 1.f};
  std::shared_ptr<std::vector<float>> wt =
      std::make_shared<std::vector<float>>(wtRaw);
  Clover::Nodes::Wavetable::WavetableOsc wavetableDC;
  wavetableDC.wavetable(wt);

  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<2> collector(3);
  Clover::Nodes::Stereo::Pan1 pan;

  wavetableDC >> pan >> collector >> crank;

  pan.pan(0.f);
  crank.turn(1);
  pan.pan(1.f);
  crank.turn(1);
  pan.pan(-1.f);
  crank.turn(1);

  EXPECT_LE(collector.frames[0][0], 1.f);
  EXPECT_LE(collector.frames[0][1], 1.f);
  EXPECT_EQ(collector.frames[1][0], 0.f);
  EXPECT_EQ(collector.frames[1][1], 1.f);
  EXPECT_EQ(collector.frames[2][0], 1.f);
  EXPECT_EQ(collector.frames[2][1], 0.f);
}

TEST(Nodes_SmokeTest, Stereo_Pan2) {
  std::vector<float> wtRaw = {1.f, 1.f, 1.f};
  std::shared_ptr<std::vector<float>> wt =
      std::make_shared<std::vector<float>>(wtRaw);
  Clover::Nodes::Wavetable::WavetableOscStereo wavetableDC;
  wavetableDC.wavetable(wt);

  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<2> collector(3);
  Clover::Nodes::Stereo::Pan2 pan;

  wavetableDC >> pan >> collector >> crank;

  pan.pan(0.f);
  crank.turn(1);
  pan.pan(1.f);
  crank.turn(1);
  pan.pan(-1.f);
  crank.turn(1);

  EXPECT_LE(collector.frames[0][0], 1.f);
  EXPECT_LE(collector.frames[0][1], 1.f);
  EXPECT_EQ(collector.frames[1][0], 0.f);
  EXPECT_EQ(collector.frames[1][1], 1.f);
  EXPECT_EQ(collector.frames[2][0], 1.f);
  EXPECT_EQ(collector.frames[2][1], 0.f);
}

TEST(Nodes_SmokeTest, Sum_Mono) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<1> collector(1);
  Clover::Nodes::Stereo::Sum1 sum;
  Clover::_Test::DCN<2> dc;

  dc >> sum >> collector >> crank;

  dc.indexBasis(0); // [0,1]
  crank.turn(1);

  EXPECT_FLOAT_EQ(collector.frames[0][0], 0.5f);
}

TEST(Nodes_SmokeTest, Sum_Stereo) {
  Clover::_Test::HandCrank crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::Nodes::Stereo::Sum2 sum;
  Clover::_Test::DCN<2> dc;

  dc >> sum >> collector >> crank;

  dc.indexBasis(0); // [0,1]
  crank.turn(1);

  EXPECT_FLOAT_EQ(collector.frames[0][0], 0.5f);
  EXPECT_FLOAT_EQ(collector.frames[0][1], 0.5f);
}
