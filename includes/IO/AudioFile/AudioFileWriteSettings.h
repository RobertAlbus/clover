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

#include <string>
#include <variant>

namespace Clover::IO::AudioFile {

// clang-format off
enum struct PcmBitDepth {
  _16,
  _24,
  _32,
  _float,
  _double,
};

enum struct PcmSampleRate {
  _441 = 44100,
  _48  = 48000,
  _96  = 96000,
  _192 = 192000,
};

enum struct PcmFileType {
  Flac,
  Wav,
};
// clang-format on

struct WriteSettingsPcm {
  WriteSettingsPcm(PcmBitDepth _bitDepth, PcmSampleRate _sampleRate,
                   PcmFileType _pcmFileType);

  WriteSettingsPcm(PcmBitDepth _bitDepth, int _sampleRate,
                   PcmFileType _pcmFileType);

  PcmBitDepth bitDepth;
  int sampleRate;
  PcmFileType pcmFileType;

  static WriteSettingsPcm cd(PcmFileType _pcmFileType = PcmFileType::Wav);
  static WriteSettingsPcm hq(PcmFileType _pcmFileType = PcmFileType::Wav);
  static WriteSettingsPcm master(PcmFileType _pcmFileType = PcmFileType::Wav);

private:
  WriteSettingsPcm();
};

struct WriteSettingsMp3 {
  WriteSettingsMp3(int _bitRate);

  int bitRate;

  static WriteSettingsMp3 _320();
  static WriteSettingsMp3 _256();
  static WriteSettingsMp3 _192();
  static WriteSettingsMp3 _96();

private:
  WriteSettingsMp3();
};

using WriteSettings = std::variant<WriteSettingsPcm, WriteSettingsMp3>;

struct WriteSpec {
  std::string const path;
  WriteSettings const writeSettings;
};

} // namespace Clover::IO::AudioFile