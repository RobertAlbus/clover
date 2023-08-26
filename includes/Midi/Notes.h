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

namespace Clover::Midi::Note {
constexpr float B_2 = 0;
constexpr float C_1 = 0;
constexpr float Cs_1 = 1;
constexpr float Db_1 = 1;
constexpr float D_1 = 2;
constexpr float Ds_1 = 3;
constexpr float Eb_1 = 3;
constexpr float E_1 = 4;
constexpr float F_1 = 5;
constexpr float Fs_1 = 6;
constexpr float Gb_1 = 6;
constexpr float G_1 = 7;
constexpr float Gs_1 = 8;
constexpr float Ab_1 = 8;
constexpr float A_1 = 9;
constexpr float As_1 = 10;
constexpr float Bb_1 = 10;
constexpr float B_1 = 11;
constexpr float C0 = 12;
constexpr float Cs0 = 13;
constexpr float Db0 = 13;
constexpr float D0 = 14;
constexpr float Ds0 = 15;
constexpr float Eb0 = 15;
constexpr float E0 = 16;
constexpr float F0 = 17;
constexpr float Fs0 = 18;
constexpr float Gb0 = 18;
constexpr float G0 = 19;
constexpr float Gs0 = 20;
constexpr float Ab0 = 20;
constexpr float A0 = 21;
constexpr float As0 = 22;
constexpr float Bb0 = 22;
constexpr float B0 = 23;
constexpr float C1 = 24;
constexpr float Cs1 = 25;
constexpr float Db1 = 25;
constexpr float D1 = 26;
constexpr float Ds1 = 27;
constexpr float Eb1 = 27;
constexpr float E1 = 28;
constexpr float F1 = 29;
constexpr float Fs1 = 30;
constexpr float Gb1 = 30;
constexpr float G1 = 31;
constexpr float Gs1 = 32;
constexpr float Ab1 = 32;
constexpr float A1 = 33;
constexpr float As1 = 34;
constexpr float Bb1 = 34;
constexpr float B1 = 35;
constexpr float C2 = 36;
constexpr float Cs2 = 37;
constexpr float Db2 = 37;
constexpr float D2 = 38;
constexpr float Ds2 = 39;
constexpr float Eb2 = 39;
constexpr float E2 = 40;
constexpr float F2 = 41;
constexpr float Fs2 = 42;
constexpr float Gb2 = 42;
constexpr float G2 = 43;
constexpr float Gs2 = 44;
constexpr float Ab2 = 44;
constexpr float A2 = 45;
constexpr float As2 = 46;
constexpr float Bb2 = 46;
constexpr float B2 = 47;
constexpr float C3 = 48;
constexpr float Cs3 = 49;
constexpr float Db3 = 49;
constexpr float D3 = 50;
constexpr float Ds3 = 51;
constexpr float Eb3 = 51;
constexpr float E3 = 52;
constexpr float F3 = 53;
constexpr float Fs3 = 54;
constexpr float Gb3 = 54;
constexpr float G3 = 55;
constexpr float Gs3 = 56;
constexpr float Ab3 = 56;
constexpr float A3 = 57;
constexpr float As3 = 58;
constexpr float Bb3 = 58;
constexpr float B3 = 59;
constexpr float C4 = 60;
constexpr float Cs4 = 61;
constexpr float Db4 = 61;
constexpr float D4 = 62;
constexpr float Ds4 = 63;
constexpr float Eb4 = 63;
constexpr float E4 = 64;
constexpr float F4 = 65;
constexpr float Fs4 = 66;
constexpr float Gb4 = 66;
constexpr float G4 = 67;
constexpr float Gs4 = 68;
constexpr float Ab4 = 68;
constexpr float A4 = 69;
constexpr float As4 = 70;
constexpr float Bb4 = 70;
constexpr float B4 = 71;
constexpr float C5 = 72;
constexpr float Cs5 = 73;
constexpr float Db5 = 73;
constexpr float D5 = 74;
constexpr float Ds5 = 75;
constexpr float Eb5 = 75;
constexpr float E5 = 76;
constexpr float F5 = 77;
constexpr float Fs5 = 78;
constexpr float Gb5 = 78;
constexpr float G5 = 79;
constexpr float Gs5 = 80;
constexpr float Ab5 = 80;
constexpr float A5 = 81;
constexpr float As5 = 82;
constexpr float Bb5 = 82;
constexpr float B5 = 83;
constexpr float C6 = 84;
constexpr float Cs6 = 85;
constexpr float Db6 = 85;
constexpr float D6 = 86;
constexpr float Ds6 = 87;
constexpr float Eb6 = 87;
constexpr float E6 = 88;
constexpr float F6 = 89;
constexpr float Fs6 = 90;
constexpr float Gb6 = 90;
constexpr float G6 = 91;
constexpr float Gs6 = 92;
constexpr float Ab6 = 92;
constexpr float A6 = 93;
constexpr float As6 = 94;
constexpr float Bb6 = 94;
constexpr float B6 = 95;
constexpr float C7 = 96;
constexpr float Cs7 = 97;
constexpr float Db7 = 97;
constexpr float D7 = 98;
constexpr float Ds7 = 99;
constexpr float Eb7 = 99;
constexpr float E7 = 100;
constexpr float F7 = 101;
constexpr float Fs7 = 102;
constexpr float Gb7 = 102;
constexpr float G7 = 103;
constexpr float Gs7 = 104;
constexpr float Ab7 = 104;
constexpr float A7 = 105;
constexpr float As7 = 106;
constexpr float Bb7 = 106;
constexpr float B7 = 107;
constexpr float C8 = 108;
constexpr float Cs8 = 109;
constexpr float Db8 = 109;
constexpr float D8 = 110;
constexpr float Ds8 = 111;
constexpr float Eb8 = 111;
constexpr float E8 = 112;
constexpr float F8 = 113;
constexpr float Fs8 = 114;
constexpr float Gb8 = 114;
constexpr float G8 = 115;
constexpr float Gs8 = 116;
constexpr float Ab8 = 116;
constexpr float A8 = 117;
constexpr float As8 = 118;
constexpr float Bb8 = 118;
constexpr float B8 = 119;
constexpr float C9 = 120;
constexpr float Cs9 = 121;
constexpr float Db9 = 121;
constexpr float D9 = 122;
constexpr float Ds9 = 123;
constexpr float Eb9 = 123;
constexpr float E9 = 124;
constexpr float F9 = 125;
constexpr float Fs9 = 126;
constexpr float Gb9 = 126;
constexpr float G9 = 127;
} // namespace Clover::Midi::Note
