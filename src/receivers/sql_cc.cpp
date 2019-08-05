/**-------------------------------------------------------------------------
 * @file sql_cc.cpp
 * @brief a hierarchical block for squelch
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
#include "receivers/sql_cc.h"
#include <gnuradio/gr_complex.h>

/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Function:
 *     make_sql_cc
 */
sql_cc::sptr sql_cc::make(double sql_level)
{
    return gnuradio::get_initial_sptr(new sql_cc(sql_level));
}

/*--------------------------------------------------------------------------
 * Function:
 *     sql_cc
 *
 *  Remarks:
 *     see prototype in sql_cc.h
 */
sql_cc::sql_cc(double sql_level)
    : gr::hier_block2("sql_cc",//const std::string &name
           gr::io_signature::make(1,1,sizeof(gr_complex)),//input_signature
           gr::io_signature::make(1,1,sizeof(gr_complex)))//output_signature
{
    // do the squelch using simple squelch 
    m_sql_level = sql_level;
    m_sql = gr::analog::simple_squelch_cc::make(m_sql_level, 0.001);

    connect( self(), 0, m_sql, 0);
    connect( m_sql, 0, self(), 0);
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~sql_cc
 *
 *  Remarks:
 *     see prototype in sql_cc.h
 */
sql_cc::~sql_cc()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     set_sql_level
 *
 *  Remarks:
 *     see prototype in sql_cc.h
 */
void sql_cc::set_sql_level(double level_db)
{
    m_sql_level = level_db;
    m_sql->set_threshold(level_db);
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_sql_level
 *
 *  Remarks:
 *     see prototype in sql_cc.h
 */
double sql_cc::get_sql_level(void)
{
    return m_sql_level;
}

