/**-------------------------------------------------------------------------
 * @file ssbtx.h
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
 * ---------------------------------------------------------------------- */
#ifndef __SSB_TX_H__
#define __SSB_TX_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <gnuradio/hier_block2.h>
#include <gnuradio/blocks/multiply_const_ff.h>
#include <gnuradio/filter/fir_filter_fcc.h>
#include <gnuradio/filter/interp_fir_filter_ccf.h>
#include <gnuradio/gr_complex.h>
#include <vector>

class ssbtx;

class ssbtx : public gr::hier_block2
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    typedef boost::shared_ptr<ssbtx> sptr;

    /*! @brief Public constructor of wfm_tx. */
    static sptr make(float input_rate, float audio_rate);


/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
protected:
    /** @brief Constructor
     *
     * @param Void.
     */
    ssbtx(float input_rate, float audio_rate);

public:
    /** @brief Deconstructor
     *
     * @param Void.
     */
    ~ssbtx();

    /** @brief update the taps in the interpolation fir filter
     *
     * @param double low frequency
     * @param double high frequency
     * @return Void.
     */
    void set_filter(double low, double high, double tw);

    /** @brief set push to talk to off
     *
     * @return Void.
     */
    void ptt_off();

    /** @brief set push to talk to on
     *
     * @return Void.
     */
    void ptt_on();

private:
    float m_quad_rate;
    int m_audio_rate;
    float m_interp_factor;
    gr::filter::fir_filter_fcc::sptr m_ssb_filter;
    gr::filter::interp_fir_filter_ccf::sptr m_interpolator_1;
    gr::filter::interp_fir_filter_ccf::sptr m_interpolator_2;

    gr::blocks::multiply_const_ff::sptr m_key_sptr;

    /** @brief get the taps for the low pass filter
     *
     * @param double - the data rate
     * @param double - low end of filter
     * @param double - high end of filter
     * @param double - transition width of filter
     * @return std::vector<gr_complex>
     */
    std::vector<gr_complex> get_taps(double rate, double low, double high, double tw);

};

#endif /* __SSB_TX_H__ */
