#include <gtest/gtest.h>

#include "_Test/Collector.h"
#include "_Test/DCN.h"
#include "_Test/HandCrank.h"
#include "_Test/Incrementor.h"

#include "NodeSimplex/Adapter/NullAdapter.h"
#include "NodeSimplex/Delay/FractionalDelayLine.h"
#include "NodeSimplex/DynamicRange/AsymClip.h"
#include "NodeSimplex/Envelope/ADSR.h"
#include "NodeSimplex/Envelope/BasicEnvelope.h"
#include "NodeSimplex/Stereo/Difference.h"
#include "NodeSimplex/Stereo/MidSideBalance.h"
#include "NodeSimplex/Stereo/Pan.h"
#include "NodeSimplex/Stereo/Sum.h"
#include "NodeSimplex/Wavetable/WavetableOscStereo.h"

TEST(NodeSimplex_SmokeTest, NullAdapter) {
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(1);
  Clover::NodeSimplex::Adapter::NullAdapter<4, 1> nullAdapter;
  Clover::_Test::DCN<4> dc;

  dc >> nullAdapter >> collector >> crank;

  crank.turn(1);

  EXPECT_EQ(collector.frames[0][0], 0.f);
}

TEST(NodeSimplex_SmokeTest, Delay_Fractional) {
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

TEST(NodeSimplex_SmokeTest, DynamicRange_Clamp) {
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> envelopeCollector(6);
  Clover::NodeSimplex::DynamicRange::AsymClip<1> clip;
  Clover::_Test::DCN<1> dc;

  dc >> clip >> envelopeCollector >> crank;

  dc.indexBasis(-1.5f);
  crank.turn(1);
  dc.indexBasis(-1.0f);
  crank.turn(1);
  dc.indexBasis(-0.5f);
  crank.turn(1);
  dc.indexBasis(0.5f);
  crank.turn(1);
  dc.indexBasis(1.0f);
  crank.turn(1);
  dc.indexBasis(1.5f);
  crank.turn(1);

  EXPECT_EQ(envelopeCollector.frames[0][0], -1.f);
  EXPECT_EQ(envelopeCollector.frames[1][0], -1.f);
  EXPECT_EQ(envelopeCollector.frames[2][0], -0.5f);

  EXPECT_EQ(envelopeCollector.frames[3][0], 0.5f);
  EXPECT_EQ(envelopeCollector.frames[4][0], 1.f);
  EXPECT_EQ(envelopeCollector.frames[5][0], 1.f);
}

TEST(NodeSimplex_SmokeTest, Envelope_Basic) {
  Clover::_Test::HandCrank<1> crank;
  Clover::NodeSimplex::Envelope::BasicEnvelope envelope;
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

TEST(NodeSimplex_SmokeTest, Envelope_ADSR) {
  Clover::_Test::HandCrank<1> crank;
  Clover::NodeSimplex::Envelope::ADSR envelope;
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

TEST(NodeSimplex_SmokeTest, Stereo_Difference) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(2);
  Clover::NodeSimplex::Stereo::Difference difference;
  Clover::_Test::DCN<2> dc;

  dc >> difference >> collector >> crank;

  dc.indexBasis(0); // [0,1]
  crank.turn(1);

  EXPECT_FLOAT_EQ(collector.frames[0][0], -0.5f);
  EXPECT_FLOAT_EQ(collector.frames[0][1], 0.5f);
}

TEST(NodeSimplex_SmokeTest, Stereo_MidSideBalance) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(3);
  Clover::NodeSimplex::Stereo::MidSideBalance ms;
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

TEST(NodeSimplex_SmokeTest, Stereo_Pan1) {
  std::vector<float> wtRaw = {1.f, 1.f, 1.f};
  std::shared_ptr<std::vector<float>> wt =
      std::make_shared<std::vector<float>>(wtRaw);
  Clover::NodeSimplex::Wavetable::WavetableOsc wavetableDC;
  wavetableDC.wavetable(wt);

  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(3);
  Clover::NodeSimplex::Stereo::Pan1 pan;

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

TEST(NodeSimplex_SmokeTest, Stereo_Pan2) {
  std::vector<float> wtRaw = {1.f, 1.f, 1.f};
  std::shared_ptr<std::vector<float>> wt =
      std::make_shared<std::vector<float>>(wtRaw);
  Clover::NodeSimplex::Wavetable::WavetableOscStereo wavetableDC;
  wavetableDC.wavetable(wt);

  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(3);
  Clover::NodeSimplex::Stereo::Pan2 pan;

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

TEST(NodeSimplex_SmokeTest, Sum_Mono) {
  Clover::_Test::HandCrank<1> crank;
  Clover::_Test::Collector<1> collector(1);
  Clover::NodeSimplex::Stereo::Sum1 sum;
  Clover::_Test::DCN<2> dc;

  dc >> sum >> collector >> crank;

  dc.indexBasis(0); // [0,1]
  crank.turn(1);

  EXPECT_FLOAT_EQ(collector.frames[0][0], 0.5f);
}

TEST(NodeSimplex_SmokeTest, Sum_Stereo) {
  Clover::_Test::HandCrank<2> crank;
  Clover::_Test::Collector<2> collector(1);
  Clover::NodeSimplex::Stereo::Sum2 sum;
  Clover::_Test::DCN<2> dc;

  dc >> sum >> collector >> crank;

  dc.indexBasis(0); // [0,1]
  crank.turn(1);

  EXPECT_FLOAT_EQ(collector.frames[0][0], 0.5f);
  EXPECT_FLOAT_EQ(collector.frames[0][1], 0.5f);
}