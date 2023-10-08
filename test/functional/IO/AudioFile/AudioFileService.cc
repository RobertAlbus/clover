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

#include <algorithm>
#include <filesystem>

#include <gtest/gtest.h>

#include "IO/AudioFile/AudioFile.h"
#include "IO/AudioFile/AudioFileService.h"
#include "IO/AudioFile/AudioFileWriteSettings.h"

using namespace Clover::IO::AudioFile;

// clang-format off
bool HAVE_TESTS_FAILED_WRITE = false;
bool HAVE_TESTS_FAILED_READ  = false;
// clang-format on

void cleanFile(std::string path) {
  if (std::filesystem::exists(path)) {
    std::filesystem::remove(path);
  }
}

TEST(IO_AudioFile_AudioFileService, WriteOne) {

  // ARRANGE
  AudioFileService service = AudioFileService::BuildInstance();

  AudioFile audioFileFixture;
  audioFileFixture.channelCount = 1;
  for (int i = 0; i < 30; i++) {
    audioFileFixture.audioData.emplace_back(0);
  }

  std::string path = "AudioFileService_Write_One.wav";
  cleanFile(path);

  WriteSpec spec(path, WriteSettingsPcm::cd());

  // ACT
  service.Write(spec, audioFileFixture);

  // ASSERT
  bool isFileCreated = std::filesystem::exists(path);
  EXPECT_TRUE(isFileCreated);

  if (!isFileCreated) {
    HAVE_TESTS_FAILED_WRITE = true;
  }

  // CLEAN
  cleanFile(path);
}

TEST(IO_AudioFile_AudioFileService, WriteMany) {

  // ARRANGE
  AudioFileService service = AudioFileService::BuildInstance();

  AudioFile audioFileFixture;
  audioFileFixture.channelCount = 1;
  for (int i = 0; i < 30; i++) {
    audioFileFixture.audioData.emplace_back(0);
  }

  std::string pathPrefix = "AudioFileService_Write_";
  std::vector<WriteSpec> specs;
  for (int i = 0; i < 3; i++) {
    std::string path = pathPrefix + std::to_string(i) + ".wav";
    cleanFile(path);
    specs.emplace_back(WriteSpec(path, WriteSettingsPcm::cd()));
  }

  // ACT
  service.Write(specs, audioFileFixture);

  for (auto spec : specs) {
    // ASSERT
    bool isFileCreated = std::filesystem::exists(spec.path);
    EXPECT_TRUE(isFileCreated);

    if (!isFileCreated) {
      HAVE_TESTS_FAILED_WRITE = true;
    }

    // CLEAN
    cleanFile(spec.path);
  }
}

TEST(IO_AudioFile_AudioFileService, ReadOne) {

  // TEST DEPENDENCY: AudioFileService.Write used to seed test fixture
  if (HAVE_TESTS_FAILED_WRITE) {
    GTEST_SKIP();
  }

  // ARRANGE
  AudioFileService service = AudioFileService::BuildInstance();

  AudioFile audioFileFixture;
  audioFileFixture.channelCount = 1;
  for (int i = 0; i < 30; i++) {
    audioFileFixture.audioData.emplace_back((i % 3) - 1);
  }

  std::string path = "AudioFileService_Read_One.wav";
  cleanFile(path);

  WriteSpec spec(
      path, WriteSettingsPcm(PcmBitDepth::_float, 48000, PcmFileType::Wav)
  );
  service.Write(spec, audioFileFixture);

  // ACT
  AudioFile audioFile = service.Read(path);

  // ASSERT
  for (int i = 0; i < audioFile.audioData.size(); i++) {
    float fixtureSignal = audioFileFixture.audioData[i];
    float fileSignal = audioFile.audioData[i];

    bool doSignalsMatch = fixtureSignal == fileSignal;
    EXPECT_TRUE(doSignalsMatch);

    if (!doSignalsMatch) {
      HAVE_TESTS_FAILED_READ = true;
    }
  }

  // CLEAN
  cleanFile(path);
}

TEST(IO_AudioFile_AudioFileService, ReadMany) {

  // TEST DEPENDENCY: AudioFileService.Write used to seed test fixture
  if (HAVE_TESTS_FAILED_WRITE) {
    GTEST_SKIP();
  }

  // ARRANGE
  AudioFileService service = AudioFileService::BuildInstance();

  AudioFile audioFileFixture;
  audioFileFixture.channelCount = 1;

  audioFileFixture.audioData.reserve(30);
  for (int i = 0; i < 30; i++) {
    float signal = ((i % 3) - 1);
    audioFileFixture.audioData.emplace_back(signal);
  }

  std::string pathPrefix = "AudioFileService_Read_";
  std::vector<WriteSpec> specs;

  specs.reserve(3);
  for (int i = 0; i < 3; i++) {
    std::string path = pathPrefix + std::to_string(i) + ".wav";
    cleanFile(path);
    specs.emplace_back(WriteSpec(
        path, WriteSettingsPcm(PcmBitDepth::_float, 48000, PcmFileType::Wav)
    ));
  }

  std::vector<std::string> paths;
  std::transform(
      specs.begin(),
      specs.end(),
      std::back_inserter(paths),
      [](const WriteSpec &spec) { return spec.path; }
  );

  service.Write(specs, audioFileFixture);

  // ACT
  std::vector<AudioFile> audioFiles = service.Read(paths);

  // ASSERT
  std::for_each(
      audioFiles.begin(),
      audioFiles.end(),
      [&audioFileFixture](const AudioFile &audioFile) {
    for (int i = 0; i < audioFile.audioData.size(); i++) {
      float fixtureSignal = audioFileFixture.audioData[i];
      float fileSignal = audioFile.audioData[i];

      EXPECT_FLOAT_EQ(fixtureSignal, fileSignal)
          << i << "  fixture: " << fixtureSignal << "  file: " << fileSignal;

      if (fixtureSignal != fileSignal) {
        HAVE_TESTS_FAILED_READ = true;
      }
    }
      });

  // CLEAN
  std::for_each(specs.begin(), specs.end(), [](const WriteSpec &spec) {
    cleanFile(spec.path);
  });
}