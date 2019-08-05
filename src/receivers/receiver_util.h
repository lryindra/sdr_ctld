/**-------------------------------------------------------------------------
 * @file receiver_util.h
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
 * ---------------------------------------------------------------------- */
#ifndef __RECEIVER_UTIL_H__ 
#define __RECEIVER_UTIL_H__ 

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <string>

class Receiver_Util
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    static const float PREF_QUAD_RATE;  /**< prefered quad rate for demodulation */
    static const float PREF_AUDIO_RATE; /**< prefered audio rate for demodulation */
    static const float PREF_TX_RATE; /**< preffered rate for transmiters. */

    /** @typedef rxopt_mode_idx
     * @brief Mode selector entries.
     */
    enum rxopt_mode_idx {
        MODE_RAW        = 0, /*!< Raw I/Q passthrough. */
        MODE_LSB        = 1, /*!< Lower side band. */
        MODE_USB        = 2, /*!< Upper side band. */
        MODE_LAST       = 3
    };
    static const std::string STR_RAW;      /**< raw */
    static const std::string STR_LSB;      /**< lower side band */
    static const std::string STR_USB;      /**< upper side band */
    static const std::string STR_Invalid; /**< invalid modulation/demodulation */
    /** @typedef filter_shape
     * @brief applied to the received signal; based on the demodulation
     */
    enum filter_shape {
                FILTER_SHAPE_SOFT = 0,   
                FILTER_SHAPE_NORMAL = 1, 
                FILTER_SHAPE_SHARP = 2 
    };
/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/

    /** @brief return true if this is a valid resample ratio
     *
     * @param input_rate - this is the data rate from the SDR
     * @param audio_rate - this is the output data rate
     * @return bool
     */
    static bool is_ratio_valid(float input_rate, float audio_rate);

private:
    /** @brief Constructor
     *
     * @param Void.
     */
    Receiver_Util();

    /** @brief Deconstructor
     *
     * @param Void.
     */
    ~Receiver_Util();

};

#endif /* __RECEIVER_UTIL_H__ */
