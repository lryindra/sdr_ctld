/**-------------------------------------------------------------------------
* @file limey_device_list.h
* @brief search this computer for Lime devices
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
#ifndef __LIMEY_DEVICE_LIST_H__
#define __LIMEY_DEVICE_LIST_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <string>
#include <vector>

class Limey_Device_List 
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    struct {
        std::string info;
        std::string serial;
        std::string name;
        bool is_mini;
    } typedef limey_device_t;
    static const double oscillator; 

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/

    /** @brief get the device list
     *
     * @param Void.
     * @return std::vector<limey_device_t>
     */
    static std::vector<limey_device_t> get_device_list();

private:
    /** @brief Constructor
     *
     * @param Void.
     */
    Limey_Device_List( );

    /** @brief Deconstructor
     *
     * @param Void.
     */
    ~Limey_Device_List();

};

#endif /* __LIMEY_DEVICE_LIST_H__ */
