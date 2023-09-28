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

#include <cstdint> // for std::int32_t
#include <vector>

enum struct ChannelConfiguration {
  None = 0,
  Mono = 1,
  Stereo = 2,
  Quadraphonic = 4,
  Surround5_1 = 6
};

struct AudioFile {
  AudioFile()
      : audioData(), channelConfig(ChannelConfiguration::None), cuePoints(),
        sampleRateHz(0) {}

  std::vector<float> audioData;
  ChannelConfiguration channelConfig;
  std::vector<std::int32_t> cuePoints;
  std::int32_t sampleRateHz;
};
