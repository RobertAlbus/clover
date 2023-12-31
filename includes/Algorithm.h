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

#include "Algo/Envelope/ADSR.h"
#include "Algo/Envelope/Basic.h"
#include "Algo/Envelope/DC.h"

#include "Algo/DynamicRange/AsymClip.h"

#include "Algo/Filter/IIR/ButterworthCoefficients.h"
#include "Algo/Filter/IIR/ChebyshevType1Coefficients.h"
#include "Algo/Filter/IIR/ChebyshevType2Coefficients.h"
#include "Algo/Filter/IIR/IIRCoefficientStrategy.h"
#include "Algo/Filter/IIR/IIRFilterDF2T.h"
#include "Algo/Filter/IIR/IIR_Coefficients.h"
#include "Algo/Filter/IIR/RbjBiquadCoefficients.h"
