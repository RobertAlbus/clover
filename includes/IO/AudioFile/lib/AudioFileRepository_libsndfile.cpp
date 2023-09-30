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
#include <variant>

#include "../AudioFile.h"
#include "AudioFileRepository_libsndfile.h"

#include "filesystem_Delete.h"
#include "lamemp3_Write.h"
#include "libsndfile_Append.h"
#include "libsndfile_Read.h"
#include "libsndfile_Write.h"

namespace Clover::IO::AudioFile {

void AudioFileRepository_libsndfile::Write(const WriteSpec &writeSpec,
                                           const AudioFile &audioFile) {
  const char *path = getPath(writeSpec);
  const WriteSettings writeSettings = getWriteSettings(writeSpec);

  std::visit(
      [path, audioFile](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, WriteSettingsPcm>) {
          auto settings = static_cast<WriteSettingsPcm>(arg);
          impl::libsndfile_Write(path, settings, audioFile);

        } else if constexpr (std::is_same_v<T, WriteSettingsMp3 &>) {
          auto settings = static_cast<WriteSettingsMp3>(arg);
          impl::lamemp3_Write(path, settings, audioFile);

        } else {
          throw std::runtime_error("Unhandled WriteSettings variant for "
                                   "AudioFileRepository.Write");
        }
      },
      writeSettings);
}

AudioFile AudioFileRepository_libsndfile::Read(const std::string &path) {
  return impl::libsndfile_Read(path.c_str());
}

void AudioFileRepository_libsndfile::Append(const WriteSpec &writeSpec,
                                            const AudioFile &audioFile) {

  const char *path = getPath(writeSpec);
  const WriteSettings writeSettings = getWriteSettings(writeSpec);

  std::visit(
      [path, audioFile](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, WriteSettingsPcm>) {
          auto settings = static_cast<WriteSettingsPcm>(arg);
          impl::libsndfile_Append(path, settings, audioFile);

        } else if constexpr (std::is_same_v<T, WriteSettingsMp3 &>) {
          auto settings = static_cast<WriteSettingsMp3>(arg);
          throw std::runtime_error("WriteSettingsMp3 variant not supported "
                                   "for AudioFileRepository.Append.");

        } else {
          throw std::runtime_error("Unhandled WriteSettings variant for "
                                   "AudioFileRepository.Append");
        }
      },
      writeSettings);
}

void AudioFileRepository_libsndfile::Delete(const std::string &path) {
  Read(path); // validate that it's an audio file
  impl::filesystem_Delete(path);
}

const char *
AudioFileRepository_libsndfile::getPath(const WriteSpec &writeSpec) {
  return writeSpec.first;
  const WriteSettings writeSettings = writeSpec.second;
}

WriteSettings
AudioFileRepository_libsndfile::getWriteSettings(const WriteSpec &writeSpec) {
  return writeSpec.second;
}

} // namespace Clover::IO::AudioFile