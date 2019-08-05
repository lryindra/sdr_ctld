/**-------------------------------------------------------------------------
 * @file ssbrx.cpp
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
*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "receivers/ssbrx.h"
#include "receivers/receiver_util.h"
#include "application/logger.h"
#include <gnuradio/gr_complex.h>
#include <gnuradio/io_signature.h>

/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Function:
 *     make_ssbrx
 */
ssbrx::sptr ssbrx::make(float input_rate, float audio_rate)
{
    return gnuradio::get_initial_sptr(new ssbrx(input_rate, audio_rate));
}

/*--------------------------------------------------------------------------
 * Function:
 *     ssbrx
 */
ssbrx::ssbrx(float input_rate, float audio_rate)
    : gr::hier_block2("ssbrx",
            gr::io_signature::make(1,1,sizeof(gr_complex)),
            gr::io_signature::make(1,1,sizeof(float)))
{
    // reduce the data rate from input_rate down to audio_rate
    m_resamp_filter = polyphase_resamp_filter::make(input_rate, audio_rate, -5000.0, 5000.0, 1000.0);
    // do the squelch using simple squelch_cc
    m_sql = sql_cc::make();
    // do demod
    m_demod = gr::blocks::complex_to_real::make(1);

    connect( self(), 0, m_resamp_filter, 0);
    connect( m_resamp_filter, 0, m_sql, 0);
    connect( m_sql, 0, m_demod, 0);
    connect( m_demod, 0, self(), 0);
    
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~ssbrx
 */
ssbrx::~ssbrx()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     set_filter
 */
void ssbrx::set_filter(double low, double high, double tw)
{
    m_resamp_filter->set_filter(low, high, tw);
}

/*--------------------------------------------------------------------------
 * Function:
 *     set_sql_level
 */
void ssbrx::set_sql_level(double level_db)
{
    m_sql->set_sql_level(level_db);
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_sql_level
 */
double ssbrx::get_sql_level(void)
{
    return m_sql->get_sql_level();
}

