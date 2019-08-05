/**-------------------------------------------------------------------------
 * @file limey_source_c.cpp
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
*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "sdr/limey_source_c.h"
#include <gnuradio/gr_complex.h>
#include <gnuradio/io_signature.h>
#include "application/utility.h"
#include "application/logger.h"

/*--------------------------------------------------------------------------
 * Function:
 *     make_Limey_Source_c
 *
 *  Remarks:
 *     see prototype in limey_source_c.h
 */
Limey_Source_c::sptr Limey_Source_c::make( std::string serial, double freq, double input_rate, double min_freq )
{
    return gnuradio::get_initial_sptr(new Limey_Source_c(serial, freq, input_rate, min_freq));
}

/*--------------------------------------------------------------------------
 * Function:
 *     Limey_Source_c 
 */
Limey_Source_c::Limey_Source_c( std::string serial, double freq, double input_rate, double min_freq  )
    : gr::hier_block2("Lime SDR Source",//const std::string &name
          gr::io_signature::make(0, 0, 0),//input_signature
          gr::io_signature::make(1,1,sizeof(gr_complex))),// output_signatur
    m_min_freq(min_freq)
{
    int pa_path_mini = 255;// None(0), high(1), low(2), wide(3), AUTO(255)
    m_chan = 0;// LMS_CH_0

    m_limey_c_sptr= gr::limesdr::source::make(
            serial, //std::string serial
            m_chan, // channel mode selelct (SISO): A(1), B(2), (A+B)MIMO(3)
            ""); // path to file if file swithc is turned on
    m_limey_c_sptr->set_sample_rate(input_rate); //set same sammple rate for both channels
    this->set_center_frequency(freq);
    m_center_freq = freq;
    m_limey_c_sptr->set_digital_filter(500000, m_chan);//digital filter set to 500 kHz
    m_limey_c_sptr->set_antenna(pa_path_mini, m_chan);//set antenna 
    m_limey_c_sptr->set_gain(69, m_chan); //set combined gain
    // set oversampling to default
    m_limey_c_sptr->calibrate(5000000, m_chan); // set calibration bandwidth
    //set fifo buffer size

    if(nullptr == m_limey_c_sptr || NULL == m_limey_c_sptr )
    {
        Logger::crit("[Limey_Source_c::Limey_Source_c] pointer to Lime SDR sink is NULL. ");
        throw "pointer to Lime SDR sink is NULL.";
    }
    connect(m_limey_c_sptr, 0, self(), 0);
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~Limey_Source_c 
 */
Limey_Source_c::~Limey_Source_c()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     set_center_frequency
 */
bool Limey_Source_c::set_center_frequency(double freq)
{
    bool rval = false;
    if( Limey_Device_List::oscillator <= freq ) 
    {
        //set nco to off
        m_limey_c_sptr->set_nco(0, m_chan); 
        //set analog bandwidth
        m_limey_c_sptr->set_bandwidth(5000000, m_chan); 
        m_limey_c_sptr->set_center_freq(freq, m_chan);
        rval = true;
        m_center_freq = freq;
    }
    else if( m_min_freq <= freq )
    {
        float nco_val = Limey_Device_List::oscillator - freq;
        double bandw = nco_val *2;
        //the current bandwidth performed the same as 60000000 bandwidth
        m_limey_c_sptr->set_nco(nco_val, m_chan);
        //set analog bandwidth
        m_limey_c_sptr->set_bandwidth(bandw, m_chan); 
        m_limey_c_sptr->set_center_freq(Limey_Device_List::oscillator, m_chan);
        rval = true;
        m_center_freq = freq;
    }
    return rval;
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_center_frequency
 */
double Limey_Source_c::get_center_frequency()
{
    return m_center_freq;
}

