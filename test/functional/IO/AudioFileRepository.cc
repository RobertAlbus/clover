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

#include "Algorithm/Wavetable/WavetableOscillatorMono.h"
#include "IO/AudioFile/AudioFile.h"
#include "IO/AudioFile/AudioFileWriteSettings.h"
#include "IO/AudioFile/lib/AudioFileRepository.h"

#include "./AudioFileRepository/AudioFileRepository_util.h"

using namespace Clover::IO::AudioFile;

TEST(AudioFileRepository_libsndfile_Integration, Write_PCM) {
  std::unique_ptr<AudioFileRepository> repo_ptr =
      AudioFileRepository::BuildInstance();
  AudioFileRepository &repository = *repo_ptr;

  AudioFile file = generateAudioFileData();

  std::vector<WriteSpec> writeSpecs = generateValidWriteSpecsFlac();
  std::vector<WriteSpec> wavWriteSpecs = generateValidWriteSpecsWav();
  writeSpecs.insert(
      writeSpecs.end(), wavWriteSpecs.begin(), wavWriteSpecs.end()
  );

  for (auto writeSpec : writeSpecs) {
    if (std::filesystem::exists(writeSpec.path)) {
      std::filesystem::remove(writeSpec.path);
    }
  }

  for (auto writeSpec : writeSpecs) {
    repository.Write(writeSpec, file);
  }

  for (auto writeSpec : writeSpecs) {
    EXPECT_TRUE(std::filesystem::exists(writeSpec.path));
    std::filesystem::remove(writeSpec.path);
  }
}

TEST(AudioFileRepository_libsndfile_Integration, Full_Wav) {
  int samplerate = 48000;
  std::string path = "./TEST.wav";
  int channelCount = 2;

  std::unique_ptr<AudioFileRepository> repo_ptr =
      AudioFileRepository::BuildInstance();
  AudioFileRepository &repository = *repo_ptr;

  AudioFile file = generateAudioFileData();

  file.channelCount = 2;
  file.sampleRateHz = samplerate;

  Clover::Wavetable::WavetableOscillatorMono<float> osc(samplerate);
  osc.freq(500);

  WriteSpec writeSpec(
      path,
      WriteSettingsPcm(
          PcmBitDepth::_float, PcmSampleRate::_48, PcmFileType::Wav
      )
  );

  repository.Write(writeSpec, file);
  AudioFile readFile = repository.Read(path);

  EXPECT_EQ(file.channelCount, readFile.channelCount);
  EXPECT_EQ(file.sampleRateHz, readFile.sampleRateHz);
  EXPECT_EQ(file.cuePoints.size(), readFile.cuePoints.size());

  for (int i = 0, end = file.cuePoints.size(); i < end; i++) {
    EXPECT_FLOAT_EQ(file.cuePoints.at(i), readFile.cuePoints.at(i));
  }

  int audioDataSize = file.audioData.size();
  for (int i = 0; i < audioDataSize; i++) {
    float fileData = readFile.audioData.at(i);
    float audioData = file.audioData.at(i);
    EXPECT_FLOAT_EQ(fileData, audioData) << i;
  }

  repository.Append(writeSpec, file);
  readFile = repository.Read(path);

  int audioDataSizeDoubled = audioDataSize * 2;

  EXPECT_EQ(readFile.audioData.size(), audioDataSizeDoubled);
  for (int i = 0; i < audioDataSizeDoubled; i++) {
    float fileData = readFile.audioData.at(i);
    float audioData = file.audioData.at(i % audioDataSize);

    EXPECT_FLOAT_EQ(fileData, audioData);
  }

  EXPECT_TRUE(std::filesystem::exists(path));
  repository.Delete(path);
  EXPECT_FALSE(std::filesystem::exists(path));
}
