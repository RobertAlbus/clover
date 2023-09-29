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

#include <filesystem>

#include <gtest/gtest.h>

#include "Algo/Wavetable/WavetableOscillatorMono.h"
#include "IO/AudioFile/AudioFile.h"
#include "IO/AudioFile/lib/AudioFileRepository_libsndfile.h"

TEST(AudioFileRepository_Wav, ShouldWrite) {
  int samplerate = 48000;
  std::string path = "./TEST.wav";
  int channelCount = 2;

  AudioFileRepositoryWav repository;
  AudioFile file;

  file.channelCount = 2;
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

  repository.Write(path, file);
  AudioFile readFile = repository.Read(path);

  EXPECT_EQ(file.channelCount, readFile.channelCount);
  EXPECT_EQ(file.sampleRateHz, readFile.sampleRateHz);
  EXPECT_EQ(file.cuePoints.size(), readFile.cuePoints.size());

  for (int i = 0, end = file.cuePoints.size(); i < end; i++) {
    EXPECT_FLOAT_EQ(file.cuePoints.at(i), readFile.cuePoints.at(i));
  }

  for (int i = 0; i < twoSeconds * channelCount; i++) {
    float fileData = readFile.audioData.at(i);
    float audioData = file.audioData.at(i);
    EXPECT_FLOAT_EQ(fileData, audioData);
  }

  repository.Append(path, file);
  readFile = repository.Read(path);

  int fourSeconds = twoSeconds * 2;

  int dataSize = fourSeconds * channelCount;
  EXPECT_EQ(readFile.audioData.size(), dataSize);
  for (int i = 0; i < dataSize; i++) {
    float fileData = readFile.audioData.at(i);
    float audioData = file.audioData.at(i % (twoSeconds * channelCount));

    EXPECT_FLOAT_EQ(fileData, audioData) << "???";
  }

  EXPECT_TRUE(std::filesystem::exists(path));
  repository.Delete(path);
  EXPECT_FALSE(std::filesystem::exists(path));
}