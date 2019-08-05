/**-------------------------------------------------------------------------
 * @file ssbrx.h
 * @brief receive single side band
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
#ifndef __SSB_RX_H__
#define __SSB_RX_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <gnuradio/hier_block2.h>
#include "receivers/polyphase_resamp_filter.h"
#include "receivers/sql_cc.h"
#include <gnuradio/blocks/complex_to_real.h>
#include <string>
#include <vector>

class ssbrx : public gr::hier_block2
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    /** shared pointer to single sideband receiver */
    typedef boost::shared_ptr<ssbrx> sptr;

    /** returns a single sideband receiver */
    static sptr make(float input_rate, float audio_rate);

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
protected:
    /** @brief Constructor
     *
     * @param input_rate - data rate of the receiver
     * @param audio_rate - data rate out to audio
     */
    ssbrx(float input_rate, float audio_rate);

public:
    /** @brief Deconstructor
     *
     */
    ~ssbrx();

    /** @brief update the taps in the interpolation fir filter
     *
     * @param low - low frequency
     * @param high - high frequency
     * @param tw - transition width
     * @return Void.
     */
    void set_filter(double low, double high, double tw);

    /** @brief sets the sql level
     *
     * @param level_db - level in db
     * @return Void.
     */
    void set_sql_level(double level_db);

    /** @brief get the sql level
     *
     * @return double
     */
    double get_sql_level(void);

private:
    polyphase_resamp_filter::sptr m_resamp_filter;
    sql_cc::sptr m_sql;
    gr::blocks::complex_to_real::sptr m_demod;

};

#endif /* __SSB_RX_H__ */
