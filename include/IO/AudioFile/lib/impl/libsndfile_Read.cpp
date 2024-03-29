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
#include "libsndfile__Util.h"

namespace Clover::IO::AudioFile::impl {

AudioFile libsndfile_Read(const std::string &path) {
  AudioFile audioFile;
  SF_INFO sfinfo;
  SNDFILE *file = sf_open(path.c_str(), SFM_READ, &sfinfo);
  throwIfFails(file, sf_error(file));

  audioFile.sampleRateHz = sfinfo.samplerate;
  audioFile.channelCount = sfinfo.channels;

  audioFile.audioData.resize(sfinfo.frames * sfinfo.channels);
  sf_count_t count = sf_read_float(
      file, audioFile.audioData.data(), audioFile.audioData.size()
  );

  if (count != static_cast<sf_count_t>(audioFile.audioData.size())) {
    throwIfFails(file, sf_error(file));
  }

  SF_CUES cues;
  if (sf_command(file, SFC_GET_CUE, &cues, sizeof(cues)) == SF_TRUE) {
    for (unsigned i = 0; i < cues.cue_count; ++i) {
      audioFile.cuePoints.push_back(cues.cue_points[i].sample_offset);
    }
  }

  if (sf_close(file) != 0) {
    int err = sf_error(file);
    if (err != SF_ERR_NO_ERROR) {
      throw std::runtime_error(sf_strerror(file));
    }
  }

  return audioFile;
}

} // namespace Clover::IO::AudioFile::impl