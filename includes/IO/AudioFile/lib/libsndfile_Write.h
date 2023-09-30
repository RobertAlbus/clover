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

#include <stdexcept>
#include <string>
#include <vector>

#include "sndfile.h"

#include "../AudioFile.h"
#include "../AudioFileWriteSettings.h"
#include "libsndfile__Util.h"

namespace Clover::IO::AudioFile::impl {

void libsndfile_Write(const char *path, const WriteSettingsPcm &writeSettings,
                      const AudioFile &audioFile) {
  SF_INFO sfinfo;
  sfinfo.samplerate = writeSettings.sampleRate;
  sfinfo.channels = audioFile.channelCount;
  sfinfo.format = getWriteSettingsFormat(writeSettings);

  SNDFILE *file = sf_open(path, SFM_WRITE, &sfinfo);
  throwIfFails(file, sf_error(file));

  sf_count_t count = sf_write_float(file, audioFile.audioData.data(),
                                    audioFile.audioData.size());

  if (count != static_cast<sf_count_t>(audioFile.audioData.size())) {
    throwIfFails(file, sf_error(file));
  }

  std::vector<SF_CUE_POINT> cuePoints;
  cuePoints.reserve(audioFile.cuePoints.size());
  for (auto &point : audioFile.cuePoints) {
    SF_CUE_POINT cuePoint;
    cuePoint.position = point;
    cuePoints.push_back(cuePoint);
  }
  sf_command(file, SFC_SET_CUE, cuePoints.data(),
             cuePoints.size() * sizeof(SF_CUE_POINT));

  sf_write_sync(file);
  throwIfFails(file, sf_close(file));
}

// SF_INFO build_SF_INFO(SF_INFO &sf_info, const AudioFile audioFile, const
// WriteSettings &writeSettings) {

//   // TODO: sample rate should come from WRiteSettings variant
//   sf_info.samplerate = audioFile.sampleRateHz;
//   sf_info.channels = audioFile.channelCount;

//   std::visit([sf_info, audioFile](auto&& arg) {

//     using T = std::decay_t<decltype(arg)>;
//     if constexpr (std::is_same_v<T, WriteSettingsPcm&>) {
//       auto settings = static_cast<WriteSettingsPcm>(arg);
//       sf_info.samplerate = settings.sampleRate;
//       sf_info.channels = audioFile.channelCount;
//       sf_info.format = getWriteSettingsFormat(settings);

//     } else if constexpr (std::is_same_v<T, WriteSettingsMp3&>) {
//       auto settings = static_cast<WriteSettingsMp3>(arg);
//       flags |= SF_FORMAT_MPEG_LAYER_III;

//     } else {
//       throw std::runtime_error("Unhandled WriteSettings variant.");

//     }
//   }, writeSettings);
// }

} // namespace Clover::IO::AudioFile::impl