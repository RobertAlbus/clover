// /*
//  * /////////
//  * // Clover
//  *
//  * Audio processing algorithms and DAG with feedback loops that do not break
//  * acyclicity.
//  *
//  * Copyright (C) 2023 Rob W. Albus
//  *
//  * This program is free software: you can redistribute it and/or modify it under
//  * the terms of the GNU General Public License as published by the Free Software
//  * Foundation, either version 3 of the License, or (at your option) any later
//  * version. This program is distributed in the hope that it will be useful, but
//  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
//  * details. You should have received a copy of the GNU General Public License
//  * along with this program. If not, see <http://www.gnu.org/licenses/>.
//  *
//  */

// #include <stdexcept>

// #include "../AudioFile.h"
// #include "../AudioFileWriteSettings.h"
// #include "WriteSpecValidator.h"

// namespace Clover::IO::AudioFile {

// void WriteSpecValidator::validate(
//     const WriteSpec writeSpec, const AudioFile audioFile
// ) {
//   if (writeSpec.path.size() < 1) {
//     throw std::runtime_error(
//         "AudioFileService cannot write a file without a path"
//     );
//   }

//   std::visit(
//       [audioFile](auto &&arg) {
//     using T = std::decay_t<decltype(arg)>;

//     if constexpr (std::is_same_v<T, WriteSettingsPcm>) {
//       const WriteSettingsPcm settings = static_cast<WriteSettingsPcm>(arg);
//       validatePcm(settings, audioFile);
//     } else if constexpr (std::is_same_v<T, WriteSettingsMp3 &>) {
//       const WriteSettingsMp3 settings = static_cast<WriteSettingsMp3>(arg);
//       validateMp3(settings, audioFile);
//     } else {
//       throw std::runtime_error("Unhandled WriteSettings variant for "
//                                "WriteSpecValidator.validate");
//     }
//       },
//       writeSpec.writeSettings);
// }

// void WriteSpecValidator::validateMp3(
//     WriteSettingsMp3 writeSettings, AudioFile audioFile
// ) {
//   bool isValidBitRate =
//       writeSettings.bitRate == 320 || writeSettings.bitRate == 256 ||
//       writeSettings.bitRate == 192 || writeSettings.bitRate == 96;

//   if (!isValidBitRate) {
//     throw std::runtime_error("Clover::IO::AudioFile::WriteSpecValidator: "
//                              "Mp3 bit rate must be 96, 192, 256, or 320.");
//   }

//   bool isValidChannelCount =
//       audioFile.channelCount == 1 || audioFile.channelCount == 2;
//   if (!isValidChannelCount) {
//     throw std::runtime_error("Clover::IO::AudioFile::WriteSpecValidator: "
//                              "Mp3 files can only be written for audio with a"
//                              "channel count of 1 or 2");
//   }
// }

// void WriteSpecValidator::validatePcm(
//     WriteSettingsPcm writeSettings, AudioFile audioFile
// ) {
//   if (writeSettings.pcmFileType == PcmFileType::Flac) {
//     validateFlac(writeSettings, audioFile);
//   } else if (writeSettings.pcmFileType == PcmFileType::Wav) {
//     validateWav(writeSettings, audioFile);
//   } else {
//     throw std::runtime_error("Clover::IO::AudioFile::WriteSpecValidator: "
//                              "Unhandled PCM file type");
//   }
// }

// void WriteSpecValidator::validateFlac(
//     WriteSettingsPcm writeSettings, AudioFile audioFile
// ) {
//   if (writeSettings.pcmFileType != PcmFileType::Flac)
//     return;

//   bool isSupportedBitDepth = writeSettings.bitDepth == PcmBitDepth::_16 ||
//                              writeSettings.bitDepth == PcmBitDepth::_24;
//   if (!isSupportedBitDepth) {
//     throw std::runtime_error("Clover::IO::AudioFile::WriteSpecValidator: "
//                              "Flac files must be 16 or 24 bit.");
//   }

//   bool isSupportedChannelCount =
//       audioFile.channelCount > 0 && audioFile.channelCount <= 9;

//   if (!isSupportedChannelCount) {
//     throw std::runtime_error("Clover::IO::AudioFile::WriteSpecValidator: "
//                              "Flac files can only be written for audio with a"
//                              "channel count between 1 and 8");
//   }
// }

// void WriteSpecValidator::validateWav(
//     WriteSettingsPcm writeSettings, AudioFile audioFile
// ) {
//   if (writeSettings.pcmFileType != PcmFileType::Wav)
//     return;

//   bool isSupportedChannelCount =
//       audioFile.channelCount > 0 && audioFile.channelCount <= 1024;

//   if (!isSupportedChannelCount) {
//     throw std::runtime_error("Clover::IO::AudioFile::WriteSpecValidator: "
//                              "Wav files can only be written for audio with a "
//                              "channel count between 1 and 1024");
//   }
// }

// } // namespace Clover::IO::AudioFile
