/**-------------------------------------------------------------------------
 * @file limey_source_c.h
 * @brief a custom driver for the Lime SDR
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
#ifndef __LIMEY_SOURCE_C_H__
#define __LIMEY_SOURCE_C_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <gnuradio/hier_block2.h>
#include <limesdr/source.h>
#include <string>
#include "sdr/limey_device_list.h"

class Limey_Source_c : public gr::hier_block2
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    typedef boost::shared_ptr<Limey_Source_c> sptr;

    static sptr make(std::string serial, double freq = 60000000, double input_rate = 1000000, double min_freq = Limey_Device_List::oscillator );

protected:
    /** @brief Constructor
     *
     * @param serial - the serial number of the LimeSDR
     * @param freq - frequency set in Hz
     * @param input_rate - sample rate in Msps
     * @param min_freq - the minimum center frequency with no seg fault
     */
    Limey_Source_c( std::string serial, double freq, double input_rate, double min_freq );

public:
/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    /** @brief Deconstructor
     *
     * @param Void.
     */
    ~Limey_Source_c();

    /** @brief Set center frequency
     *
     * @param freq - Frequency to set in Hz
     * @return bool - return true if freq is valid
     */
    bool set_center_frequency(double freq);

    /** @brief Get center frequency
     *
     * @return double - the center frequency
     */
    double get_center_frequency();

private:
    gr::limesdr::source::sptr m_limey_c_sptr;
    size_t m_chan;
    double m_center_freq;
    double m_min_freq;

};

#endif /* __LIMEY_SOURCE_C_H__ */
