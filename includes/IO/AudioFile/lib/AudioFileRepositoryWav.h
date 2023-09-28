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
#include "../AudioFileRepository.h"

struct AudioFileRepositoryWav : public AudioFileRepository {
  void Write(const std::string &filePath, const AudioFile &audioFile) override {
    // Implementation using libsndfile
  }

  AudioFile Read(const std::string &filePath) override {
    // Implementation using libsndfile
    AudioFile f;
    return f;
  }

  void Append(const std::string &filePath,
              const AudioFile &audioFile) override {
    // Implementation using libsndfile
  }

  void Delete(const std::string &filePath) override {
    // Implementation using standard C++ file operations
  }
};
