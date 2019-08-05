/**-------------------------------------------------------------------------
 * @file sql_cc.h
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
* ----------------------------------------------------------------------- */
#ifndef __SQL_CC_H__
#define __SQL_CC_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <gnuradio/hier_block2.h>
#include <gnuradio/analog/simple_squelch_cc.h>

class sql_cc;

class sql_cc : public gr::hier_block2
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    /** shared pointer to the squelch block */
    typedef boost::shared_ptr<sql_cc> sptr;

    static sptr make(double sql_level=-150.0);

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
protected:
    /** @brief Constructor
     *
     * @param sql_level - default squelch level
     */
    sql_cc(double sql_level);

public:
    /** @brief Deconstructor
     *
     */
    ~sql_cc();

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
    double m_sql_level;
    gr::analog::simple_squelch_cc::sptr m_sql;

};

#endif /* __SQL_CC_H__ */
