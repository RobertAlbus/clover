#pragma once

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

#include <cstring>
#include <stdexcept>
#include <vector>

#include "Algo/Wavetable/WavetableOscillatorMono.h"
#include "IO/AudioFile/AudioFile.h"
#include "IO/AudioFile/AudioFileWriteSettings.h"

#include <random>
#include <string>

std::string randString(int length) {
  std::string s;
  std::mt19937 r{std::random_device{}()};
  std::uniform_int_distribution<> d(48, 122);
  while (s.size() < length) {
    char c = d(r);
    if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
      s += c;
  }
  return s;
}

using namespace Clover::IO::AudioFile;

AudioFile generateAudioFileData() {
  int samplerate = 48000;
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

  return file;
}

std::vector<WriteSettings>
generateWriteSettingsPcmCombos(std::vector<PcmSampleRate> sampleRates,
                               std::vector<PcmBitDepth> bitDepths,
                               std::vector<PcmFileType> fileTypes) {

  std::vector<WriteSettings> writeSettings;
  for (PcmFileType filetype : fileTypes) {
    for (PcmBitDepth bitDepth : bitDepths) {
      for (PcmSampleRate sampleRate : sampleRates) {
        writeSettings.emplace_back(
            WriteSettingsPcm(bitDepth, sampleRate, filetype));
      }
    }
  }

  return writeSettings;
}

std::vector<WriteSpec>
generateWriteSpecs(std::vector<WriteSettings> writeSettings,
                   std::string fileSiffix) {
  std::vector<WriteSpec> writeSpecs;
  for (auto writeSetting : writeSettings) {
    WriteSpec spec(randString(10) + fileSiffix, writeSetting);
    writeSpecs.emplace_back(spec);
  }

  return writeSpecs;
}

std::vector<WriteSpec> generateValidWriteSpecsWav() {
  std::vector<PcmSampleRate> sampleRates;
  sampleRates.emplace_back(PcmSampleRate::_441);
  sampleRates.emplace_back(PcmSampleRate::_48);
  sampleRates.emplace_back(PcmSampleRate::_96);
  sampleRates.emplace_back(PcmSampleRate::_192);

  std::vector<PcmBitDepth> bitDepths;
  bitDepths.emplace_back(PcmBitDepth::_16);
  bitDepths.emplace_back(PcmBitDepth::_24);
  bitDepths.emplace_back(PcmBitDepth::_32);
  bitDepths.emplace_back(PcmBitDepth::_double);
  bitDepths.emplace_back(PcmBitDepth::_float);

  std::vector<PcmFileType> fileTypes;
  fileTypes.emplace_back(PcmFileType::Wav);

  std::vector<WriteSettings> writeSettings =
      generateWriteSettingsPcmCombos(sampleRates, bitDepths, fileTypes);

  return generateWriteSpecs(writeSettings, ".wav");
}

std::vector<WriteSpec> generateValidWriteSpecsFlac() {
  std::vector<PcmSampleRate> sampleRates;
  sampleRates.emplace_back(PcmSampleRate::_441);
  sampleRates.emplace_back(PcmSampleRate::_48);
  sampleRates.emplace_back(PcmSampleRate::_96);
  sampleRates.emplace_back(PcmSampleRate::_192);

  // flac above 24 is not supported in libsndfile
  std::vector<PcmBitDepth> bitDepths;
  bitDepths.emplace_back(PcmBitDepth::_16);
  bitDepths.emplace_back(PcmBitDepth::_24);

  // optional dependency xiph may solve this?
  // https://xiph.org/flac/2022/09/09/flac-1-4-0-released.html#:~:text=FLAC%20can%20now%20encode%20and,not%2032%20bit%20float%20samples.

  std::vector<PcmFileType> fileTypes;
  fileTypes.emplace_back(PcmFileType::Flac);

  std::vector<WriteSettings> writeSettings =
      generateWriteSettingsPcmCombos(sampleRates, bitDepths, fileTypes);

  return generateWriteSpecs(writeSettings, ".wav");
}
