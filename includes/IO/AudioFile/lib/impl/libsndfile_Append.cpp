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

#include "sndfile.h"

#include "../../AudioFile.h"
#include "../../AudioFileWriteSettings.h"
#include "libsndfile__Util.h"

namespace Clover::IO::AudioFile::impl {

void libsndfile_Append(
    const std::string &path,
    const WriteSettingsPcm &writeSettings,
    const AudioFile &audioFile
) {

  SF_INFO sfinfo;
  SNDFILE *file = sf_open(path.c_str(), SFM_RDWR, &sfinfo);

  throwIfFails(file, sf_error(file));

  bool sampleRatesMismatch = sfinfo.samplerate != audioFile.sampleRateHz;
  if (sampleRatesMismatch) {
    sf_close(file);
    throw std::runtime_error(
        "Incompatible sample rate when appending audio to file: path=[" +
        std::string(path) + "]"
    );
  }

  bool channelCountMismatch = sfinfo.channels != audioFile.channelCount;
  if (channelCountMismatch) {
    sf_close(file);
    throw std::runtime_error(
        "Incompatible channel count when appending audio to file: path=[" +
        std::string(path) + "]"
    );
  }

  sf_seek(file, 0, SEEK_END);
  sf_count_t count = sf_write_float(
      file, audioFile.audioData.data(), audioFile.audioData.size()
  );

  if (count != static_cast<sf_count_t>(audioFile.audioData.size())) {
    throwIfFails(file, sf_error(file));
  }

  sf_write_sync(file);
  throwIfFails(file, sf_close(file));
}

} // namespace Clover::IO::AudioFile::impl