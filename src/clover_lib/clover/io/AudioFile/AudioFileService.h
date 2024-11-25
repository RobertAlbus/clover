// #pragma once

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

// #include <memory>
// #include <vector>

// // internal dependencies
// #include "Clover/IO/AudioCallback.h"
// #include "lib/AudioFileRepository.h"
// #include "lib/WriteSpecValidator.h"

// namespace Clover::IO::AudioFile {

// struct AudioFileService {
//   static AudioFileService BuildInstance();

//   // single op
//   void Write(const WriteSpec &writeSpec, const AudioFile &audioFile);
//   AudioFile Read(const std::string &filePath);
//   void Append(const WriteSpec &writeSpec, const AudioFile &audioFile);
//   void Delete(const std::string &filePath);

//   void Write(const WriteSpec &writeSpec, const std::function<AudioCallbackStatus(AudioCallbackArguments)> &audioCallback, int channelCount, int duration, int sampleRateHz);

//   // multi op
//   void
//   Write(const std::vector<WriteSpec> &writeSpecs, const AudioFile &audioFile);
//   std::vector<AudioFile> Read(const std::vector<std::string> &filePaths);
//   void
//   Append(const std::vector<WriteSpec> &writeSpecs, const AudioFile &audioFile);
//   void Delete(const std::vector<std::string> &filePaths);

// private:
//   std::unique_ptr<AudioFileRepository> repostiory;
//   WriteSpecValidator validator;

//   AudioFileService();
// };

// } // namespace Clover::IO::AudioFile
