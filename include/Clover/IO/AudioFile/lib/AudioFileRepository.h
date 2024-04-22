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

#include <memory>
#include <string>

#include "../AudioFile.h"
#include "../AudioFileWriteSettings.h"

namespace Clover::IO::AudioFile {

struct AudioFileRepository {
  virtual ~AudioFileRepository() = default;

  static std::unique_ptr<AudioFileRepository> BuildInstance();

  virtual void
  Write(const WriteSpec &writeSpec, const AudioFile &audioFile) = 0;
  virtual AudioFile Read(const std::string &filePath) = 0;
  virtual void
  Append(const WriteSpec &writeSpec, const AudioFile &audioFile) = 0;
  virtual void Delete(const std::string &filePath) = 0;
};

} // namespace Clover::IO::AudioFile