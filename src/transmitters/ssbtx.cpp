/**-------------------------------------------------------------------------
 * @file ssbtx.cpp
 * @brief transmit single side band
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
#include "transmitters/ssbtx.h"
#include "receivers/receiver_util.h"
#include "application/logger.h"
#include <gnuradio/filter/firdes.h>
#include <string>
#include <gnuradio/io_signature.h>


/*--------------------------------------------------------------------------
 * Function:
 *     make
 */
ssbtx::sptr ssbtx::make(float input_rate, float audio_rate)
{
    return gnuradio::get_initial_sptr(new ssbtx(input_rate, audio_rate));
}

/*--------------------------------------------------------------------------
 * Function:
 *     ssbtx
 */
ssbtx::ssbtx(float input_rate, float audio_rate)
    : gr::hier_block2("ssb_tx",
         gr::io_signature::make(1, 1, sizeof(float)), // input_signature
         gr::io_signature::make(1, 1, sizeof(gr_complex)))//output_signature
{
    m_audio_rate = int(audio_rate);
    m_quad_rate = input_rate;
    Logger::debug("[ssbtx::ssbtx] quad_rate is "+std::to_string(m_quad_rate));
    Logger::debug("[ssbtx::ssbtx] audio_rate is "+std::to_string(m_audio_rate));

    // crate push-to-talk
    m_key_sptr = gr::blocks::multiply_const_ff::make(0);

    // fir filter to single side band (upper side band is default)
    double low = 100;
    double high = 2300;
    double tw = std::abs(high - low) * 0.2;
    std::vector<gr_complex> taps_c = get_taps( m_audio_rate, low, high, tw);
    m_ssb_filter = gr::filter::fir_filter_fcc::make(1,taps_c);

    float mid_rate = m_audio_rate;
    int interp_rate = int(m_quad_rate) % int(mid_rate);
    if(0 != interp_rate)
    {
        Logger::crit("[ssbtx::ssbtx] quad_rate must be an integer multiple of prefered rate."+std::to_string(m_quad_rate)+" % "+std::to_string(m_audio_rate ));
    }

    bool do_interp = (m_audio_rate != m_quad_rate);
    int interp_factor = 1;
    if(do_interp)
    {
        interp_factor = int(m_quad_rate / m_audio_rate);
    }

    int interp_1 = 1;
    if(9 < interp_factor){
        if(0 == (interp_factor % 9))
            interp_1 = 9;
        else if(0 == (interp_factor % 8))
            interp_1 = 8;
        else if(0 == (interp_factor % 7))
            interp_1 = 7;
        else if(0 == (interp_factor % 6))
            interp_1 = 6;
        else if(0 == (interp_factor % 5))
            interp_1 = 5;
        else if(0 == (interp_factor % 4))
            interp_1 = 4;
        else if(0 == (interp_factor % 3))
            interp_1 = 3;
        else if(0 == (interp_factor % 2))
            interp_1 = 2;
        else 
            interp_1 = 1;
    }
    int interp_2 = (int)(interp_factor / interp_1);
    if(interp_2 > 9)
    {
        Logger::warn("[ssbtx::ssbtx] interpolation factors are large: "+std::to_string(interp_1)+", "+std::to_string(interp_2));
    }

    std::vector<float> taps_interp_1 = gr::filter::firdes::low_pass_2(interp_1, m_audio_rate*interp_1, (m_audio_rate * 0.3), (m_audio_rate * 0.2), 100 );
    Logger::debug("[ssbtx::ssbtx] interpolation factor 1: "+std::to_string(interp_1)+"  number of taps: "+std::to_string(taps_interp_1.size()));
    m_interpolator_1 = gr::filter::interp_fir_filter_ccf::make(interp_1, taps_interp_1);

    std::vector<float> taps_interp_2 = gr::filter::firdes::low_pass_2(interp_2, (m_audio_rate*interp_1*interp_2), (m_audio_rate*interp_1 * 0.3), (m_audio_rate*interp_1 * 0.2), 100 );
    Logger::debug("[ssbtx::ssbtx] interpolation factor 2: "+std::to_string(interp_2)+"  number of taps: "+std::to_string(taps_interp_2.size()));
    m_interpolator_2 = gr::filter::interp_fir_filter_ccf::make(interp_2, taps_interp_2);

    try
    {
        connect( self(), 0, m_key_sptr, 0);
        connect( m_key_sptr, 0, m_ssb_filter, 0);
        connect( m_ssb_filter, 0, m_interpolator_1, 0);
        connect( m_interpolator_1, 0, m_interpolator_2, 0);
        connect( m_interpolator_2, 0, self(), 0);
    }
    catch(std::invalid_argument& e)
    {
        Logger::crit("[ssbtx::ssbtx] You are trying to connect blocks with different sized ports (eg. connecting a complex to a float). "+std::string(e.what()));
    }
    Logger::debug("[ssbtx::ssbtx] complete.");
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~ssbtx
 */
ssbtx::~ssbtx()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     set_filter
 */
void ssbtx::set_filter(double low, double high, double tw)
{
    if( (high - low) < 0)
    {
        Logger::warn("[ssbtx::set_filter] Invalid filter. high = "+std::to_string(high)+"; low = "+std::to_string(low));
    }
    else
    {
        std::vector<gr_complex> taps_c = get_taps( m_audio_rate, low, high, tw);
        m_ssb_filter->set_taps(taps_c);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *     ptt_off
 */
void ssbtx::ptt_off()
{
    m_key_sptr->set_k(0);
}

/*--------------------------------------------------------------------------
 * Function:
 *     ptt_on
 */
void ssbtx::ptt_on()
{
    m_key_sptr->set_k(1);
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_taps
 */
std::vector<gr_complex> ssbtx::get_taps(double rate, double low, double high, double tw)
{
    /* generate taps */
    std::vector<gr_complex> taps = gr::filter::firdes::complex_band_pass(1, rate, low, high, tw);

    return taps;
}
