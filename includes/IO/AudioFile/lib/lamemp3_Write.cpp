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

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

// available as a transitive dependency via libsndfile
#include "lame/lame.h"

#include "../AudioFile.h"
#include "../AudioFileWriteSettings.h"
#include "lamemp3_Write.h"

namespace Clover::IO::AudioFile::impl {

void lamemp3_Write(const char *path, const WriteSettingsMp3 &writeSettings,
                   const AudioFile &audioFile) {

  lame_t lame = lame_init();
  if (!lame) {
    throw std::runtime_error(
        "lamemp3_Write: Could not initialize lamemp3 encoder");
  }

  for (auto sample : audioFile.audioData) {
    if (sample > 1.f || sample < -1.f) {
      throw std::runtime_error(
          "lamemp3_Write: Audio data must be normalized to -1..1");
    }
  }

  lame_set_in_samplerate(lame, audioFile.sampleRateHz);
  lame_set_VBR(lame, vbr_off); // Disable variable bit rate
  lame_set_brate(lame, writeSettings.bitRate);
  lame_set_num_channels(lame, audioFile.channelCount);

  if (lame_init_params(lame) < 0) {
    lame_close(lame);
    throw std::runtime_error(
        "lamemp3_Write: Could not initialize lamemp3 encoder");
  }

  const int frameCount = audioFile.audioData.size() / audioFile.channelCount;
  const int bufferSize = frameCount * 1.25 + 7200;
  std::vector<unsigned char> mp3_buffer(bufferSize);

  int mp3_bytes = lame_encode_buffer_ieee_float(
      lame,
      // Left channel of interleaved buffer
      audioFile.audioData.data(),
      // Right channel of interleaved buffer or nullptr for mono
      (audioFile.channelCount == 2) ? audioFile.audioData.data() + 1 : nullptr,
      frameCount, mp3_buffer.data(), mp3_buffer.size());

  if (mp3_bytes < 0) {
    lame_close(lame);
    throw std::runtime_error(
        "lamemp3_Write: Could not initialize lamemp3 encoder");
  }

  std::ofstream mp3_file(path, std::ios::binary);
  mp3_file.write((char *)mp3_buffer.data(), mp3_bytes);

  int flush_bytes =
      lame_encode_flush(lame, mp3_buffer.data(), mp3_buffer.size());

  mp3_file.write((char *)mp3_buffer.data(), flush_bytes);
  mp3_file.close();

  lame_close(lame);
}

} // namespace Clover::IO::AudioFile::impl