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

#include <gtest/gtest.h>

#include "Clover.h"

TEST(Midi_Notes, NotesShouldExist) {
  EXPECT_EQ(Clover::Midi::Note::B_2, 0);
  EXPECT_EQ(Clover::Midi::Note::C_1, 0);
  EXPECT_EQ(Clover::Midi::Note::Cs_1, 1);
  EXPECT_EQ(Clover::Midi::Note::Db_1, 1);
  EXPECT_EQ(Clover::Midi::Note::D_1, 2);
  EXPECT_EQ(Clover::Midi::Note::Ds_1, 3);
  EXPECT_EQ(Clover::Midi::Note::Eb_1, 3);
  EXPECT_EQ(Clover::Midi::Note::E_1, 4);
  EXPECT_EQ(Clover::Midi::Note::F_1, 5);
  EXPECT_EQ(Clover::Midi::Note::Fs_1, 6);
  EXPECT_EQ(Clover::Midi::Note::Gb_1, 6);
  EXPECT_EQ(Clover::Midi::Note::G_1, 7);
  EXPECT_EQ(Clover::Midi::Note::Gs_1, 8);
  EXPECT_EQ(Clover::Midi::Note::Ab_1, 8);
  EXPECT_EQ(Clover::Midi::Note::A_1, 9);
  EXPECT_EQ(Clover::Midi::Note::As_1, 10);
  EXPECT_EQ(Clover::Midi::Note::Bb_1, 10);
  EXPECT_EQ(Clover::Midi::Note::B_1, 11);
  EXPECT_EQ(Clover::Midi::Note::C0, 12);
  EXPECT_EQ(Clover::Midi::Note::Cs0, 13);
  EXPECT_EQ(Clover::Midi::Note::Db0, 13);
  EXPECT_EQ(Clover::Midi::Note::D0, 14);
  EXPECT_EQ(Clover::Midi::Note::Ds0, 15);
  EXPECT_EQ(Clover::Midi::Note::Eb0, 15);
  EXPECT_EQ(Clover::Midi::Note::E0, 16);
  EXPECT_EQ(Clover::Midi::Note::F0, 17);
  EXPECT_EQ(Clover::Midi::Note::Fs0, 18);
  EXPECT_EQ(Clover::Midi::Note::Gb0, 18);
  EXPECT_EQ(Clover::Midi::Note::G0, 19);
  EXPECT_EQ(Clover::Midi::Note::Gs0, 20);
  EXPECT_EQ(Clover::Midi::Note::Ab0, 20);
  EXPECT_EQ(Clover::Midi::Note::A0, 21);
  EXPECT_EQ(Clover::Midi::Note::As0, 22);
  EXPECT_EQ(Clover::Midi::Note::Bb0, 22);
  EXPECT_EQ(Clover::Midi::Note::B0, 23);
  EXPECT_EQ(Clover::Midi::Note::C1, 24);
  EXPECT_EQ(Clover::Midi::Note::Cs1, 25);
  EXPECT_EQ(Clover::Midi::Note::Db1, 25);
  EXPECT_EQ(Clover::Midi::Note::D1, 26);
  EXPECT_EQ(Clover::Midi::Note::Ds1, 27);
  EXPECT_EQ(Clover::Midi::Note::Eb1, 27);
  EXPECT_EQ(Clover::Midi::Note::E1, 28);
  EXPECT_EQ(Clover::Midi::Note::F1, 29);
  EXPECT_EQ(Clover::Midi::Note::Fs1, 30);
  EXPECT_EQ(Clover::Midi::Note::Gb1, 30);
  EXPECT_EQ(Clover::Midi::Note::G1, 31);
  EXPECT_EQ(Clover::Midi::Note::Gs1, 32);
  EXPECT_EQ(Clover::Midi::Note::Ab1, 32);
  EXPECT_EQ(Clover::Midi::Note::A1, 33);
  EXPECT_EQ(Clover::Midi::Note::As1, 34);
  EXPECT_EQ(Clover::Midi::Note::Bb1, 34);
  EXPECT_EQ(Clover::Midi::Note::B1, 35);
  EXPECT_EQ(Clover::Midi::Note::C2, 36);
  EXPECT_EQ(Clover::Midi::Note::Cs2, 37);
  EXPECT_EQ(Clover::Midi::Note::Db2, 37);
  EXPECT_EQ(Clover::Midi::Note::D2, 38);
  EXPECT_EQ(Clover::Midi::Note::Ds2, 39);
  EXPECT_EQ(Clover::Midi::Note::Eb2, 39);
  EXPECT_EQ(Clover::Midi::Note::E2, 40);
  EXPECT_EQ(Clover::Midi::Note::F2, 41);
  EXPECT_EQ(Clover::Midi::Note::Fs2, 42);
  EXPECT_EQ(Clover::Midi::Note::Gb2, 42);
  EXPECT_EQ(Clover::Midi::Note::G2, 43);
  EXPECT_EQ(Clover::Midi::Note::Gs2, 44);
  EXPECT_EQ(Clover::Midi::Note::Ab2, 44);
  EXPECT_EQ(Clover::Midi::Note::A2, 45);
  EXPECT_EQ(Clover::Midi::Note::As2, 46);
  EXPECT_EQ(Clover::Midi::Note::Bb2, 46);
  EXPECT_EQ(Clover::Midi::Note::B2, 47);
  EXPECT_EQ(Clover::Midi::Note::C3, 48);
  EXPECT_EQ(Clover::Midi::Note::Cs3, 49);
  EXPECT_EQ(Clover::Midi::Note::Db3, 49);
  EXPECT_EQ(Clover::Midi::Note::D3, 50);
  EXPECT_EQ(Clover::Midi::Note::Ds3, 51);
  EXPECT_EQ(Clover::Midi::Note::Eb3, 51);
  EXPECT_EQ(Clover::Midi::Note::E3, 52);
  EXPECT_EQ(Clover::Midi::Note::F3, 53);
  EXPECT_EQ(Clover::Midi::Note::Fs3, 54);
  EXPECT_EQ(Clover::Midi::Note::Gb3, 54);
  EXPECT_EQ(Clover::Midi::Note::G3, 55);
  EXPECT_EQ(Clover::Midi::Note::Gs3, 56);
  EXPECT_EQ(Clover::Midi::Note::Ab3, 56);
  EXPECT_EQ(Clover::Midi::Note::A3, 57);
  EXPECT_EQ(Clover::Midi::Note::As3, 58);
  EXPECT_EQ(Clover::Midi::Note::Bb3, 58);
  EXPECT_EQ(Clover::Midi::Note::B3, 59);
  EXPECT_EQ(Clover::Midi::Note::C4, 60);
  EXPECT_EQ(Clover::Midi::Note::Cs4, 61);
  EXPECT_EQ(Clover::Midi::Note::Db4, 61);
  EXPECT_EQ(Clover::Midi::Note::D4, 62);
  EXPECT_EQ(Clover::Midi::Note::Ds4, 63);
  EXPECT_EQ(Clover::Midi::Note::Eb4, 63);
  EXPECT_EQ(Clover::Midi::Note::E4, 64);
  EXPECT_EQ(Clover::Midi::Note::F4, 65);
  EXPECT_EQ(Clover::Midi::Note::Fs4, 66);
  EXPECT_EQ(Clover::Midi::Note::Gb4, 66);
  EXPECT_EQ(Clover::Midi::Note::G4, 67);
  EXPECT_EQ(Clover::Midi::Note::Gs4, 68);
  EXPECT_EQ(Clover::Midi::Note::Ab4, 68);
  EXPECT_EQ(Clover::Midi::Note::A4, 69);
  EXPECT_EQ(Clover::Midi::Note::As4, 70);
  EXPECT_EQ(Clover::Midi::Note::Bb4, 70);
  EXPECT_EQ(Clover::Midi::Note::B4, 71);
  EXPECT_EQ(Clover::Midi::Note::C5, 72);
  EXPECT_EQ(Clover::Midi::Note::Cs5, 73);
  EXPECT_EQ(Clover::Midi::Note::Db5, 73);
  EXPECT_EQ(Clover::Midi::Note::D5, 74);
  EXPECT_EQ(Clover::Midi::Note::Ds5, 75);
  EXPECT_EQ(Clover::Midi::Note::Eb5, 75);
  EXPECT_EQ(Clover::Midi::Note::E5, 76);
  EXPECT_EQ(Clover::Midi::Note::F5, 77);
  EXPECT_EQ(Clover::Midi::Note::Fs5, 78);
  EXPECT_EQ(Clover::Midi::Note::Gb5, 78);
  EXPECT_EQ(Clover::Midi::Note::G5, 79);
  EXPECT_EQ(Clover::Midi::Note::Gs5, 80);
  EXPECT_EQ(Clover::Midi::Note::Ab5, 80);
  EXPECT_EQ(Clover::Midi::Note::A5, 81);
  EXPECT_EQ(Clover::Midi::Note::As5, 82);
  EXPECT_EQ(Clover::Midi::Note::Bb5, 82);
  EXPECT_EQ(Clover::Midi::Note::B5, 83);
  EXPECT_EQ(Clover::Midi::Note::C6, 84);
  EXPECT_EQ(Clover::Midi::Note::Cs6, 85);
  EXPECT_EQ(Clover::Midi::Note::Db6, 85);
  EXPECT_EQ(Clover::Midi::Note::D6, 86);
  EXPECT_EQ(Clover::Midi::Note::Ds6, 87);
  EXPECT_EQ(Clover::Midi::Note::Eb6, 87);
  EXPECT_EQ(Clover::Midi::Note::E6, 88);
  EXPECT_EQ(Clover::Midi::Note::F6, 89);
  EXPECT_EQ(Clover::Midi::Note::Fs6, 90);
  EXPECT_EQ(Clover::Midi::Note::Gb6, 90);
  EXPECT_EQ(Clover::Midi::Note::G6, 91);
  EXPECT_EQ(Clover::Midi::Note::Gs6, 92);
  EXPECT_EQ(Clover::Midi::Note::Ab6, 92);
  EXPECT_EQ(Clover::Midi::Note::A6, 93);
  EXPECT_EQ(Clover::Midi::Note::As6, 94);
  EXPECT_EQ(Clover::Midi::Note::Bb6, 94);
  EXPECT_EQ(Clover::Midi::Note::B6, 95);
  EXPECT_EQ(Clover::Midi::Note::C7, 96);
  EXPECT_EQ(Clover::Midi::Note::Cs7, 97);
  EXPECT_EQ(Clover::Midi::Note::Db7, 97);
  EXPECT_EQ(Clover::Midi::Note::D7, 98);
  EXPECT_EQ(Clover::Midi::Note::Ds7, 99);
  EXPECT_EQ(Clover::Midi::Note::Eb7, 99);
  EXPECT_EQ(Clover::Midi::Note::E7, 100);
  EXPECT_EQ(Clover::Midi::Note::F7, 101);
  EXPECT_EQ(Clover::Midi::Note::Fs7, 102);
  EXPECT_EQ(Clover::Midi::Note::Gb7, 102);
  EXPECT_EQ(Clover::Midi::Note::G7, 103);
  EXPECT_EQ(Clover::Midi::Note::Gs7, 104);
  EXPECT_EQ(Clover::Midi::Note::Ab7, 104);
  EXPECT_EQ(Clover::Midi::Note::A7, 105);
  EXPECT_EQ(Clover::Midi::Note::As7, 106);
  EXPECT_EQ(Clover::Midi::Note::Bb7, 106);
  EXPECT_EQ(Clover::Midi::Note::B7, 107);
  EXPECT_EQ(Clover::Midi::Note::C8, 108);
  EXPECT_EQ(Clover::Midi::Note::Cs8, 109);
  EXPECT_EQ(Clover::Midi::Note::Db8, 109);
  EXPECT_EQ(Clover::Midi::Note::D8, 110);
  EXPECT_EQ(Clover::Midi::Note::Ds8, 111);
  EXPECT_EQ(Clover::Midi::Note::Eb8, 111);
  EXPECT_EQ(Clover::Midi::Note::E8, 112);
  EXPECT_EQ(Clover::Midi::Note::F8, 113);
  EXPECT_EQ(Clover::Midi::Note::Fs8, 114);
  EXPECT_EQ(Clover::Midi::Note::Gb8, 114);
  EXPECT_EQ(Clover::Midi::Note::G8, 115);
  EXPECT_EQ(Clover::Midi::Note::Gs8, 116);
  EXPECT_EQ(Clover::Midi::Note::Ab8, 116);
  EXPECT_EQ(Clover::Midi::Note::A8, 117);
  EXPECT_EQ(Clover::Midi::Note::As8, 118);
  EXPECT_EQ(Clover::Midi::Note::Bb8, 118);
  EXPECT_EQ(Clover::Midi::Note::B8, 119);
  EXPECT_EQ(Clover::Midi::Note::C9, 120);
  EXPECT_EQ(Clover::Midi::Note::Cs9, 121);
  EXPECT_EQ(Clover::Midi::Note::Db9, 121);
  EXPECT_EQ(Clover::Midi::Note::D9, 122);
  EXPECT_EQ(Clover::Midi::Note::Ds9, 123);
  EXPECT_EQ(Clover::Midi::Note::Eb9, 123);
  EXPECT_EQ(Clover::Midi::Note::E9, 124);
  EXPECT_EQ(Clover::Midi::Note::F9, 125);
  EXPECT_EQ(Clover::Midi::Note::Fs9, 126);
  EXPECT_EQ(Clover::Midi::Note::Gb9, 126);
  EXPECT_EQ(Clover::Midi::Note::G9, 127);
}
