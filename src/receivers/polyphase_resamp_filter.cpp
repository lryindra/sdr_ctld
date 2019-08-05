/**-------------------------------------------------------------------------
 * @file polyphase_resamp_filter.cpp
 * @brief a hierarchical block for polyphase filter and resamp filter
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
#include "receivers/polyphase_resamp_filter.h"
#include "receivers/receiver_util.h"
#include "application/logger.h"
#include <gnuradio/filter/firdes.h>

/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
const int polyphase_resamp_filter::filter_size = 32;

/*--------------------------------------------------------------------------
 * Function:
 *     make_polyphase_resamp_filter
 *
 */
polyphase_resamp_filter::sptr polyphase_resamp_filter::make(float input_rate, float audio_rate, double low, double high, double tw)
{
    return gnuradio::get_initial_sptr(new polyphase_resamp_filter(input_rate, audio_rate, low, high, tw));
}

/*--------------------------------------------------------------------------
 * Function:
 *     polyphase_resamp_filter
 *
 *  Remarks:
 *     see prototype in polyphase_resamp_filter.h
 */
polyphase_resamp_filter::polyphase_resamp_filter(float input_rate, float audio_rate, double low, double high, double tw)
    : gr::hier_block2("polyphase_resamp_filter",//const std::string &name
           gr::io_signature::make(1,1,sizeof(gr_complex)),//input_signature
           gr::io_signature::make(1,1,sizeof(gr_complex)))//output_signature
{
    m_input_rate = input_rate;
    int quad_rate = int(m_input_rate);
    m_audio_rate = int(audio_rate);
    if(!Receiver_Util::is_ratio_valid(quad_rate, audio_rate))
    {
        Logger::crit("[polyphase_resamp_filter::polyphase_resamp_filter] quad_rate must be an integer multiple of audio_rate."+std::to_string(quad_rate)+" % "+std::to_string(m_audio_rate));
        throw "error in quad_rate";
    }
    float decim_factor = get_decim_factor(quad_rate, m_audio_rate);
    Logger::debug("[polyphase_resamp_filter::polyphase_resamp_filter] input_rate:"+std::to_string(input_rate)+", audio_rate:"+std::to_string(audio_rate)+",  quad_rate:"+std::to_string(quad_rate)+",  decim_factor:"+std::to_string(decim_factor));

    // do the I & Q resample with a pfb_arb_resampler_ccf
    std::vector<float> resamp_taps = get_resamp_filter_taps(decim_factor, polyphase_resamp_filter::filter_size);
    m_resamp_filter = gr::filter::pfb_arb_resampler_ccf::make(decim_factor,resamp_taps, polyphase_resamp_filter::filter_size);
    // do a complex fir filter with complex_band_pass taps
    std::vector<gr_complex> filter_taps = get_fir_filter_taps( m_audio_rate, low, high, tw);
    m_filter = gr::filter::fir_filter_ccc::make(1, filter_taps);

    connect( self(), 0, m_resamp_filter, 0);
    connect( m_resamp_filter, 0, m_filter, 0); 
    connect( m_filter, 0, self(), 0);
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~polyphase_resamp_filter
 *
 *  Remarks:
 *     see prototype in polyphase_resamp_filter.h
 */
polyphase_resamp_filter::~polyphase_resamp_filter()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     set_filter
 *
 *  Remarks:
 *     see prototype in polyphase_resamp_filter.h
 */
void polyphase_resamp_filter::set_filter(double low, double high, double tw)
{
    std::vector<gr_complex> filter_taps = get_fir_filter_taps( m_audio_rate, low, high, tw);
    m_filter->set_taps(filter_taps); 
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_decim_factor
 *
 *  Remarks:
 *     see prototype in polyphase_resamp_filter.h
 */
float polyphase_resamp_filter::get_decim_factor(int quad_rate, int audio_rate)
{
    bool do_decim = (audio_rate != quad_rate);
    float audio = (float)audio_rate;
    float quad = (float)quad_rate;
    float decim_factor = 1;
    if(do_decim)
    {
        decim_factor = audio / quad;
    }
    return decim_factor;
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_resamp_filter_taps
 *
 *  Remarks:
 *     see prototype in polyphase_resamp_filter.h
 */
std::vector<float> polyphase_resamp_filter::get_resamp_filter_taps(float decim_factor, unsigned int size)
{
    double cutoff = decim_factor > 1.0 ? 0.4 : 0.4*decim_factor;
    double tw = decim_factor > 1.0 ? 0.2 : 0.2*decim_factor;
    return gr::filter::firdes::low_pass(size,size,cutoff,tw);
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_fir_filter_taps
 *
 *  Remarks:
 *     see prototype in polyphase_resamp_filter.h
 */
std::vector<gr_complex> polyphase_resamp_filter::get_fir_filter_taps(double sample_freq, double low, double high, double transition_width)
{
    if(low < -0.95*sample_freq/2.0)
    {
        low = -0.95*sample_freq/2.0;
    }
    if(high > 0.95*sample_freq/2.0)
    {
        high = 0.95*sample_freq/2.0;
    }
    return gr::filter::firdes::complex_band_pass(1.0, sample_freq, low, high, transition_width);
}

