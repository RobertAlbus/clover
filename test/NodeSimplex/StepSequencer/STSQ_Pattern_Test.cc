#include <vector>

#include <gtest/gtest.h>

#include "Clover.h"

TEST(NodeSimplex_StepSequencer_Pattern, ShouldCreatePatternFromVector) {

  STSQ_Pattern<float> pattern;

  std::vector<float> stepData{0, 1, 2, 3, 4};

  pattern.from(stepData, 100.2);

  EXPECT_EQ(pattern[0].data, 0.f);
  EXPECT_EQ(pattern[1].data, 1.f);
  EXPECT_EQ(pattern[2].data, 2.f);
  EXPECT_EQ(pattern[3].data, 3.f);
  EXPECT_EQ(pattern[4].data, 4.f);
}

TEST(NodeSimplex_StepSequencer_Pattern,
     ShouldRoundStartTimesToNearestWholeNumber) {

  STSQ_Pattern<float> pattern;

  std::vector<float> stepData{0, 0, 0, 0, 0};

  pattern.from(stepData, 100.2);

  EXPECT_EQ(pattern[0].start, 0);
  EXPECT_EQ(pattern[1].start, 100);
  EXPECT_EQ(pattern[2].start, 200);
  EXPECT_EQ(pattern[3].start, 301);
  EXPECT_EQ(pattern[4].start, 401);
}

TEST(NodeSimplex_StepSequencer_Pattern,
     ShouldIdenticalWhenInitializedFromAnother) {

  std::vector<STSQ_Pattern<float>> patterns;
  patterns.resize(1);

  std::vector<float> stepData{0, 1, 2};

  patterns[0].from(stepData, 100.3);

  STSQ_Pattern<float> extraPattern = patterns[0];
  patterns.emplace_back(extraPattern);

  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(patterns[i][0].data, 0.);
    EXPECT_EQ(patterns[i][1].data, 1.);
    EXPECT_EQ(patterns[i][2].data, 2.);

    EXPECT_EQ(patterns[i][0].start, 0);
    EXPECT_EQ(patterns[i][1].start, 100);
    EXPECT_EQ(patterns[i][2].start, 201);
  }
}

TEST(NodeSimplex_StepSequencer_Pattern, TotalDurationShouldNotBeRounded) {
  STSQ_Pattern<float> pattern;
  std::vector<float> stepData{0, 1, 2};

  float duration = 100.3;

  pattern.from(stepData, duration);
  EXPECT_EQ(pattern.totalDuration, duration * pattern.size());
}

TEST(NodeSimplex_StepSequencer_Pattern, ShouldBeIndexable) {
  STSQ_Pattern<float> pattern;
  std::vector<float> stepData{0, 1};

  pattern.from(stepData, 100.3);

  EXPECT_EQ(pattern[0].data, 0);
  EXPECT_EQ(pattern[0].start, 0);

  EXPECT_EQ(pattern[1].data, 1);
  EXPECT_EQ(pattern[1].start, 100);
}

TEST(NodeSimplex_StepSequencer_Pattern, ShouldHaveExpectedMembers) {
  STSQ_Pattern<float> pattern;
  std::vector<float> stepData{0, 1};

  pattern.from(stepData, 100.3);

  EXPECT_EQ(pattern.size(), 2);
}
