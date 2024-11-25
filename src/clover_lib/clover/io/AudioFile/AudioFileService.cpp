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

// #include "AudioFileService.h"
// #include "lib/WriteSpecValidator.h"

// namespace Clover::IO::AudioFile {

// AudioFileService AudioFileService::BuildInstance() {
//   return AudioFileService();
// }

// AudioFileService::AudioFileService()
//     : repostiory(AudioFileRepository::BuildInstance()) {}

// // single op
// void AudioFileService::Write(
//     const WriteSpec &writeSpec, const AudioFile &audioFile
// ) {
//   validator.validate(writeSpec, audioFile);
//   repostiory->Write(writeSpec, audioFile);
// }
// AudioFile AudioFileService::Read(const std::string &filePath) {
//   return repostiory->Read(filePath);
// }
// void AudioFileService::Append(
//     const WriteSpec &writeSpec, const AudioFile &audioFile
// ) {
//   validator.validate(writeSpec, audioFile);
//   repostiory->Append(writeSpec, audioFile);
// }
// void AudioFileService::Delete(const std::string &filePath) {
//   try {
//     repostiory->Delete(filePath);
//   } catch (std::runtime_error error) {
//     printf(error.what());
//   }
// }


// void AudioFileService::Write(
//     const WriteSpec &writeSpec,
//     const AudioCallback &audioCallback,
//     int channelCount,
//     int duration,
//     int sampleRateHz
// ) {
//   int audioDataSize = channelCount * duration;

//   std::vector<float> audioData;
//   audioData.resize(audioDataSize, 0.f);

//   std::vector<float> in {};
//   in.resize(channelCount, 0.f);

//   for (int i = 0; i < duration; i++) {
//     audioCallback({
//         .currentClockSample = i,
//         .input = &(in[0]),
//         .output = &(audioData[ i * channelCount]),
//         .numChannelsInput = 0,
//         .numChannelsOutput = channelCount
//     });
    
//   }
//   IO::AudioFile::AudioFile audioFile {
//       .audioData = audioData,
//       .channelCount = channelCount,
//       .sampleRateHz = sampleRateHz
//   };
//   auto fileService = IO::AudioFile::AudioFileService::BuildInstance();

//   fileService.Write(writeSpec, audioFile);
// }


// // multi op
// void AudioFileService::Write(
//     const std::vector<WriteSpec> &writeSpecs, const AudioFile &audioFile
// ) {
//   for (const WriteSpec writeSpec : writeSpecs) {
//     Write(writeSpec, audioFile);
//   }
// }
// std::vector<AudioFile>
// AudioFileService::Read(const std::vector<std::string> &filePaths) {
//   std::vector<AudioFile> audioFiles;
//   for (const std::string path : filePaths) {
//     audioFiles.emplace_back(Read(path));
//   }

//   return audioFiles;
// }
// void AudioFileService::Append(
//     const std::vector<WriteSpec> &writeSpecs, const AudioFile &audioFile
// ) {
//   for (const WriteSpec writeSpec : writeSpecs) {
//     Append(writeSpec, audioFile);
//   }
// }
// void AudioFileService::Delete(const std::vector<std::string> &filePaths) {
//   for (const std::string path : filePaths) {
//     Delete(path);
//   }
// }

// } // namespace Clover::IO::AudioFile
