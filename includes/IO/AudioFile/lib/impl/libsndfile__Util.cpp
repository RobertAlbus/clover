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

#include "../../AudioFileWriteSettings.h"

namespace Clover::IO::AudioFile::impl {

void throwIfFails(SNDFILE *sndfile, int resultCode) {
  bool hasError = resultCode != SF_ERR_NO_ERROR;
  bool hasFile = !!sndfile;

  const char *errorMessage = sf_strerror(sndfile);

  if (hasFile && hasError) {
    sf_close(sndfile);
  }
  if (hasError) {
    throw std::runtime_error(errorMessage);
  }
}

int getWriteSettingsFormat(const WriteSettingsPcm &writeSettings) {
  int flags = 0;

  int bitDepth = 0;

  switch (writeSettings.bitDepth) {
  case PcmBitDepth::_16:
    flags |= SF_FORMAT_PCM_16;
    break;
  case PcmBitDepth::_24:
    flags |= SF_FORMAT_PCM_24;
    break;
  case PcmBitDepth::_32:
    flags |= SF_FORMAT_PCM_32;
    break;
  case PcmBitDepth::_float:
    flags |= SF_FORMAT_FLOAT;
    break;
  case PcmBitDepth::_double:
    flags |= SF_FORMAT_DOUBLE;
    break;
  default:
    flags |= SF_FORMAT_PCM_16;
    break;
  }

  switch (writeSettings.pcmFileType) {
  case PcmFileType::Wav:
    flags |= SF_FORMAT_WAV;
    break;
  case PcmFileType::Flac:
    flags |= SF_FORMAT_FLAC;
    break;
  }

  return flags;
}

} // namespace Clover::IO::AudioFile::impl