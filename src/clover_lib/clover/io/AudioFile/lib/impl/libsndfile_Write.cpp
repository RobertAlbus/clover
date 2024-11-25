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

// #include <filesystem>
// #include <stdexcept>
// #include <vector>

// #include "sndfile.h"

// #include "../../AudioFile.h"
// #include "../../AudioFileWriteSettings.h"
// #include "libsndfile__Util.h"

// namespace Clover::IO::AudioFile::impl {

// void libsndfile_Write(
//     const std::string &path,
//     const WriteSettingsPcm &writeSettings,
//     const AudioFile &audioFile
// ) {

//   std::filesystem::path path_obj(path);
//   std::filesystem::path dir_path = path_obj.parent_path();
//   if (!dir_path.empty() && !std::filesystem::exists(dir_path)) {
//       std::filesystem::create_directories(dir_path);
//   }

//   SF_INFO sfinfo;
//   sfinfo.samplerate = writeSettings.sampleRate;
//   sfinfo.channels = audioFile.channelCount;
//   sfinfo.format = getWriteSettingsFormat(writeSettings);

//   SNDFILE *file = sf_open(path.c_str(), SFM_WRITE, &sfinfo);
//   throwIfFails(file, sf_error(file));

//   sf_count_t count = sf_write_float(
//       file, audioFile.audioData.data(), audioFile.audioData.size()
//   );

//   if (count != static_cast<sf_count_t>(audioFile.audioData.size())) {
//     throwIfFails(file, sf_error(file));
//   }

//   std::vector<SF_CUE_POINT> cuePoints;
//   cuePoints.reserve(audioFile.cuePoints.size());
//   for (auto &point : audioFile.cuePoints) {
//     SF_CUE_POINT cuePoint;
//     cuePoint.position = point;
//     cuePoints.push_back(cuePoint);
//   }
//   sf_command(
//       file,
//       SFC_SET_CUE,
//       cuePoints.data(),
//       cuePoints.size() * sizeof(SF_CUE_POINT)
//   );

//   sf_write_sync(file);
//   throwIfFails(file, sf_close(file));
// }

// } // namespace Clover::IO::AudioFile::impl