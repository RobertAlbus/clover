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

#include <variant>

// clang-format off
enum struct AudioFilePcmBitDepth {
  _16 = 16,
  _24 = 24,
  _32 = 32
};

enum struct AudioFilePcmSampleRate {
  _441 = 44100,
  _48  = 48000,
  _96  = 96000,
  _192 = 192000,
};

enum struct AudioFilePcmType {
  Flac,
  Wav,
};
// clang-format on

struct AudioFileWriteSettingsPcm {
  AudioFileWriteSettingsPcm(AudioFilePcmBitDepth _bitDepth,
                            AudioFilePcmSampleRate _sampleRate,
                            AudioFilePcmType _pcmFileType)
      : bitDepth(_bitDepth), sampleRate(static_cast<int>(_sampleRate)),
        pcmFileType(_pcmFileType) {}

  AudioFileWriteSettingsPcm(AudioFilePcmBitDepth _bitDepth, int _sampleRate,
                            AudioFilePcmType _pcmFileType)
      : bitDepth(_bitDepth), sampleRate(_sampleRate),
        pcmFileType(_pcmFileType) {}

  AudioFilePcmBitDepth bitDepth;
  int sampleRate;
  AudioFilePcmType pcmFileType;

  static AudioFileWriteSettingsPcm
  cd(AudioFilePcmType _pcmFileType = AudioFilePcmType::Wav) {
    return AudioFileWriteSettingsPcm(
        AudioFilePcmBitDepth::_16, AudioFilePcmSampleRate::_441, _pcmFileType);
  }

  static AudioFileWriteSettingsPcm
  hq(AudioFilePcmType _pcmFileType = AudioFilePcmType::Wav) {
    return AudioFileWriteSettingsPcm(AudioFilePcmBitDepth::_24,
                                     AudioFilePcmSampleRate::_48, _pcmFileType);
  }

  static AudioFileWriteSettingsPcm
  master(AudioFilePcmType _pcmFileType = AudioFilePcmType::Wav) {
    return AudioFileWriteSettingsPcm(
        AudioFilePcmBitDepth::_32, AudioFilePcmSampleRate::_192, _pcmFileType);
  }

private:
  AudioFileWriteSettingsPcm() {}
};

struct AudioFileWriteSettingsMp3 {
  AudioFileWriteSettingsMp3(int _bitRate) : bitRate(_bitRate) {}
  int bitRate;

  static AudioFileWriteSettingsMp3 _320() {
    return AudioFileWriteSettingsMp3(320000);
  }

  static AudioFileWriteSettingsMp3 _256() {
    return AudioFileWriteSettingsMp3(256000);
  }

  static AudioFileWriteSettingsMp3 _196() {
    return AudioFileWriteSettingsMp3(196000);
  }

  static AudioFileWriteSettingsMp3 _96() {
    return AudioFileWriteSettingsMp3(96000);
  }

private:
  AudioFileWriteSettingsMp3() {}
};

using AudioFileWriteSettings =
    std::variant<AudioFileWriteSettingsPcm, AudioFileWriteSettingsMp3>;
