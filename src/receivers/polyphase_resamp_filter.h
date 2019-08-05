/**-------------------------------------------------------------------------
 * @file polyphase_resamp_filter.h
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
* ----------------------------------------------------------------------- */
#ifndef __PFB_RESAMP_FLT_H__
#define __PFB_RESAMP_FLT_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <gnuradio/hier_block2.h>
#include <gnuradio/filter/pfb_arb_resampler_ccf.h>
#include <gnuradio/filter/fir_filter_ccc.h>
#include <vector>
#include <gnuradio/gr_complex.h>

class polyphase_resamp_filter;

class polyphase_resamp_filter : public gr::hier_block2
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    /** shared pointer to functional block to down sample received data */
    typedef boost::shared_ptr<polyphase_resamp_filter> sptr;

    /** pointer to a functional block to down sample received data */
    static sptr make(float input_rate, float audio_rate, double low, double high, double tw);


/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
protected:
    /** @brief Constructor
     *
     * @param input_rate - this is the data rate from the SDR
     * @param audio_rate - this is the output data rate
     * @param low - this is the low frequency of the bandpass filter
     * @param high - high frequency of the bandpass filter
     * @param tw - transition width of the bandpass fitler
     */
    polyphase_resamp_filter(float input_rate, float audio_rate, double low, double high, double tw);

public:
    /** @brief Deconstructor
     *
     */
    ~polyphase_resamp_filter();

    /** @brief update the taps in the bandpass filter
     *
     * @param low - low frequency
     * @param high - high frequency
     * @param tw - transiton width
     * @return Void.
     */
    void set_filter(double low, double high, double tw);

private:
    static const int filter_size;
    float m_input_rate;
    int m_audio_rate;
    gr::filter::pfb_arb_resampler_ccf::sptr m_resamp_filter;
    gr::filter::fir_filter_ccc::sptr m_filter;

    /** @brief return the decimation factor
     *
     * @param int quad_rate
     * @param int audio_rate
     * @return float
     */
    float get_decim_factor(int quad_rate, int audio_rate);

    /** @brief get the taps for the resamp filter
     *
     * @param float - result of get_decim_factor function
     * @param unsigned int - filter size
     * @return std::vector<float>
     */
    std::vector<float> get_resamp_filter_taps(float decim_factor, unsigned int size);

    /** @brief return the taps for the fir filter
     *
     * @param double sample_freq
     * @param double low
     * @param double high
     * @param double transition_width
     * @return std::vector<gr_complex> 
     */
    std::vector<gr_complex> get_fir_filter_taps(double sample_freq, double low, double high, double transition_width);

};

#endif /* __PFB_RESAMP_FLT_H__ */
