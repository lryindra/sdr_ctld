/**-------------------------------------------------------------------------
 * @file receiver_util.cpp
 * @brief utility for the receiver class
 *
 * Copyright 2019 Free Software Foundation, Inc.
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "receivers/receiver_util.h"

const float Receiver_Util::PREF_QUAD_RATE = 250000.0;
const float Receiver_Util::PREF_AUDIO_RATE = 125000.0;
const std::string Receiver_Util::STR_RAW = "RAW";
const std::string Receiver_Util::STR_LSB = "LSB";
const std::string Receiver_Util::STR_USB = "USB";
const std::string Receiver_Util::STR_Invalid = "Invalid";

/*--------------------------------------------------------------------------
 * Function:
 *     is_ratio_valid
 */
bool Receiver_Util::is_ratio_valid(float input_rate, float audio_rate)
{
    return (0 == (int(input_rate) % int(audio_rate)));
}

