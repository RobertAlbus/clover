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

#include <filesystem> // for std::filesystem::remove
#include <stdexcept>
#include <string>
#include <vector>

#include "sndfile.h"

#include "../AudioFile.h"
#include "../AudioFileRepository.h"

struct AudioFileRepositoryWav : public AudioFileRepository {
  void Write(const std::string &filePath, const AudioFile &audioFile) override {
    SF_INFO sfinfo;
    sfinfo.samplerate = audioFile.sampleRateHz;
    sfinfo.channels = static_cast<int>(audioFile.channelConfig);
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    SNDFILE *outfile = sf_open(filePath.c_str(), SFM_WRITE, &sfinfo);
    if (!outfile) {
      int err = sf_error(outfile);
      if (err != SF_ERR_NO_ERROR) {
        throw std::runtime_error(sf_strerror(outfile));
      }
    }

    sf_count_t count = sf_write_float(outfile, audioFile.audioData.data(),
                                      audioFile.audioData.size());

    if (count != static_cast<sf_count_t>(audioFile.audioData.size())) {
      int err = sf_error(outfile);
      sf_close(outfile);
      if (err != SF_ERR_NO_ERROR) {
        throw std::runtime_error(sf_strerror(outfile));
      }
    }

    std::vector<SF_CUE_POINT> cuePoints;
    cuePoints.reserve(audioFile.cuePoints.size());
    for (auto &point : audioFile.cuePoints) {
      SF_CUE_POINT cuePoint;
      cuePoint.position = point;
      cuePoints.push_back(cuePoint);
    }
    sf_command(outfile, SFC_SET_CUE, cuePoints.data(),
               cuePoints.size() * sizeof(SF_CUE_POINT));

    sf_write_sync(outfile);
    int err = sf_close(outfile);
    if (err != SF_ERR_NO_ERROR) {
      throw std::runtime_error(sf_strerror(outfile));
    }
  }

  AudioFile Read(const std::string &filePath) override {
    AudioFile audioFile;
    SF_INFO sfinfo;
    SNDFILE *infile = sf_open(filePath.c_str(), SFM_READ, &sfinfo);
    if (!infile) {
      int err = sf_error(infile);
      if (err != SF_ERR_NO_ERROR) {
        throw std::runtime_error(sf_strerror(infile));
      }
    }

    audioFile.sampleRateHz = sfinfo.samplerate;
    audioFile.channelConfig =
        static_cast<ChannelConfiguration>(sfinfo.channels);

    std::vector<float> tempAudioData(sfinfo.frames * sfinfo.channels);
    sf_count_t count =
        sf_read_float(infile, tempAudioData.data(), tempAudioData.size());

    if (count != static_cast<sf_count_t>(tempAudioData.size())) {
      int err = sf_error(infile);
      if (err != SF_ERR_NO_ERROR) {
        sf_close(infile);
        throw std::runtime_error(sf_strerror(infile));
      }
    }

    audioFile.audioData = std::move(tempAudioData);

    // Reading cue points
    SF_CUES cues;
    if (sf_command(infile, SFC_GET_CUE, &cues, sizeof(cues)) == SF_TRUE) {
      for (unsigned i = 0; i < cues.cue_count; ++i) {
        audioFile.cuePoints.push_back(cues.cue_points[i].sample_offset);
      }
    }

    if (sf_close(infile) != 0) {
      int err = sf_error(infile);
      if (err != SF_ERR_NO_ERROR) {
        throw std::runtime_error(sf_strerror(infile));
      }
    }

    return audioFile;
  }

  void Append(const std::string &filePath,
              const AudioFile &audioFile) override {
    // Implementation using libsndfile
  }

  void Delete(const std::string &filePath) override {
    if (!std::filesystem::remove(filePath)) {
      throw std::runtime_error("Failed to delete the audio file.");
    }
  }
};
