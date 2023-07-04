#include <gtest/gtest.h>

#include "Algo/Delay/Fractional.h"

TEST(Algorithm_Delay_Fractional, ShouldInitializeWithZeroDelay) {
  Clover::Delay::FractionalDelay<float, 1> delay(512);

  for (int i = 0; i < 512; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), f);
  }
}

TEST(Algorithm_Delay_Fractional, ShouldInitializeWithEmptyBuffer) {
  Clover::Delay::FractionalDelay<float, 1> delay(5, 5.f);

  std::array<float, 1> emptyFrame{0.f};

  for (int i = 0; i < 5; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), emptyFrame);
  }

  std::array<float, 1> firstFrame{1.f};
  EXPECT_EQ(delay.process(emptyFrame), firstFrame);
}

TEST(Algorithm_Delay_Fractional, ShouldDelayFractionally) {
  Clover::Delay::FractionalDelay<float, 1> delay(512);
  delay.delay(0.5);

  std::array<float, 1> firstFrame{0};
  delay.process(firstFrame);

  for (int i = 0; i <= 512; i++) {
    std::array<float, 1> input{i + 1.f};
    std::array<float, 1> expectedOutput{
        (static_cast<float>(i + 1) + static_cast<float>(i)) / 2.F};
    EXPECT_EQ(delay.process(input), expectedOutput);
  }
}

TEST(Algorithm_Delay_Fractional, NegativeDelayTimeSaturatesAtZero) {
  Clover::Delay::FractionalDelay<float, 1> delay(512);
  delay.delay(-10.f);

  for (int i = 0; i < 512; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), f);
  }
}

TEST(Algorithm_Delay_Fractional, ExcessiveDelayTimeSaturatesAtMax) {
  Clover::Delay::FractionalDelay<float, 1> delay(5);
  delay.delay(10.f);

  std::array<float, 1> emptyFrame{0};
  for (int i = 0; i < 5; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), emptyFrame);
  }

  std::array<float, 1> firstFrame{1};
  EXPECT_EQ(delay.process(emptyFrame), firstFrame);
}

TEST(Algorithm_Delay_Fractional, LongDelayTimeWorks) {
  int fiveMinutes = 48000 * 60 * 5;
  Clover::Delay::FractionalDelay<float, 1> delay(fiveMinutes);
  delay.delay(static_cast<float>(fiveMinutes));

  std::array<float, 1> emptyFrame{0.f};
  std::array<float, 1> firstFrame{1.f};
  std::array<float, 1> secondFrame{2.f};

  for (long int i = 0; i < fiveMinutes; i++) {
    std::array<float, 1> f{i + 1.f};
    EXPECT_EQ(delay.process(f), emptyFrame);
  }

  EXPECT_EQ(delay.process(emptyFrame), firstFrame);
  EXPECT_EQ(delay.process(emptyFrame), secondFrame);
}
