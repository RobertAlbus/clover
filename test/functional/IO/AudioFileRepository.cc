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

#include "Algo/Wavetable/WavetableOscillatorMono.h"
#include "IO/AudioFile/AudioFile.h"
#include "IO/AudioFile/lib/AudioFileRepositoryWav.h"

TEST(AudioFileRepository_Wav, ShouldWrite) {
  int samplerate = 48000;

  AudioFileRepositoryWav repository;
  AudioFile file;

  file.channelConfig = ChannelConfiguration::Stereo;
  file.sampleRateHz = samplerate;

  Clover::Wavetable::WavetableOscillatorMono<float> osc(samplerate);
  osc.freq(500);

  int twoSeconds = samplerate * 2;
  file.audioData.reserve(twoSeconds);

  for (int i = 0; i < twoSeconds; i++) {
    float signal = osc.process();
    file.audioData.emplace_back(signal); // L
    file.audioData.emplace_back(signal); // R
  }

  repository.Write("./TEST.wav", file);
  AudioFile readFile = repository.Read("./TEST.wav");

  ASSERT_EQ(file.channelConfig, readFile.channelConfig);
  ASSERT_EQ(file.sampleRateHz, readFile.sampleRateHz);
  ASSERT_EQ(file.cuePoints.size(), readFile.cuePoints.size());

  for (int i = 0, end = file.cuePoints.size(); i < end; i++) {
    ASSERT_FLOAT_EQ(file.cuePoints.at(i), readFile.cuePoints.at(i));
  }

  for (int i = 0; i < twoSeconds; i++) {
    ASSERT_FLOAT_EQ(file.audioData.at(i), readFile.audioData.at(i));
  }
}