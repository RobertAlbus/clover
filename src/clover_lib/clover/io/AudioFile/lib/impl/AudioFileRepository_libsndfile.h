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

#include <string>

#include "../../AudioFile.h"
#include "../AudioFileRepository.h"

namespace Clover::IO::AudioFile {

struct AudioFileRepository_libsndfile : public AudioFileRepository {
  void Write(const WriteSpec &writeSpec, const AudioFile &audioFile) override;
  AudioFile Read(const std::string &path) override;
  void Append(const WriteSpec &writeSpec, const AudioFile &audioFile) override;
  void Delete(const std::string &path) override;
};

} // namespace Clover::IO::AudioFile