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

// #include <utility>
// #include <variant>

// #include "AudioFileWriteSettings.h"

// namespace Clover::IO::AudioFile {

// WriteSettingsPcm::WriteSettingsPcm(
//     PcmBitDepth _bitDepth, PcmSampleRate _sampleRate, PcmFileType _pcmFileType
// )
//     : bitDepth(_bitDepth), sampleRate(static_cast<int>(_sampleRate)),
//       pcmFileType(_pcmFileType) {}

// WriteSettingsPcm::WriteSettingsPcm(
//     PcmBitDepth _bitDepth, int _sampleRate, PcmFileType _pcmFileType
// )
//     : bitDepth(_bitDepth), sampleRate(_sampleRate), pcmFileType(_pcmFileType) {}

// WriteSettingsPcm WriteSettingsPcm::cd(PcmFileType _pcmFileType) {
//   return WriteSettingsPcm(PcmBitDepth::_16, PcmSampleRate::_441, _pcmFileType);
// }

// WriteSettingsPcm WriteSettingsPcm::hq(PcmFileType _pcmFileType) {
//   return WriteSettingsPcm(PcmBitDepth::_24, PcmSampleRate::_48, _pcmFileType);
// }

// WriteSettingsPcm WriteSettingsPcm::master(PcmFileType _pcmFileType) {
//   return WriteSettingsPcm(PcmBitDepth::_32, PcmSampleRate::_192, _pcmFileType);
// }

// WriteSettingsPcm::WriteSettingsPcm() {}

// WriteSettingsMp3::WriteSettingsMp3(int _bitRate) : bitRate(_bitRate) {}
// WriteSettingsMp3 WriteSettingsMp3::_320() { return WriteSettingsMp3(320); }
// WriteSettingsMp3 WriteSettingsMp3::_256() { return WriteSettingsMp3(256); }
// WriteSettingsMp3 WriteSettingsMp3::_192() { return WriteSettingsMp3(192); }
// WriteSettingsMp3 WriteSettingsMp3::_96() { return WriteSettingsMp3(96); }

// WriteSettingsMp3::WriteSettingsMp3() {}

// } // namespace Clover::IO::AudioFile