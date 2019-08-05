/**-------------------------------------------------------------------------
* @file limey_device_list.cpp
* @brief 
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
#include "sdr/limey_device_list.h"
#include <lime/LMS7002M_parameters.h> //LMS_GetDeviceList((lms_info_str_t*) list) to get the lime devices
#include <lime/LimeSuite.h>
#include "application/utility.h"
#include "application/logger.h"

/*-------------------------------------------------------------------------
 * Type Definitions
 * ----------------------------------------------------------------------*/
const double Limey_Device_List::oscillator = 30000000;

/*--------------------------------------------------------------------------
 * Function:
 *     get_device_list
 */
std::vector<Limey_Device_List::limey_device_t> Limey_Device_List::get_device_list()
{
    std::vector<limey_device_t> rlist;

    int device_count = LMS_GetDeviceList(NULL);
    if(device_count < 0)
    {
        std::string error_str(LMS_GetLastErrorMessage());
        Logger::crit("[Limey_Device_List::get_device_list] "+error_str);
        throw error_str;
    }
    lms_info_str_t device_list[ device_count ];
    if( LMS_GetDeviceList(device_list) < 0)
    {
        std::string error_str(LMS_GetLastErrorMessage());
        Logger::crit("[Limey_Device_List::get_device_list] "+error_str);
        throw error_str;
    }

    for(int i=0; i < device_count; i++)
    {
        std::string dev_str(device_list[i]);
        limey_device_t dev;
        dev.info = dev_str;
        Logger::debug("[Limey_Device_List::get_device_list] dev["+std::to_string(i)+"]: "+dev_str);
        // find the serial number
        dev.serial = Utility::get_substring(dev_str,"serial=",",");
        // find device name
        size_t idx = dev_str.find(",");
        if(idx == std::string::npos || idx >= dev_str.size())
        {
            dev.name = "";
            dev.is_mini = false;
        }
        else
        {
            dev.name = dev_str.substr(0,idx);
            size_t idx_mini = dev.name.find("Mini");
            if(idx_mini == std::string::npos || idx_mini >= dev.name.size())
            {
                dev.is_mini = false;
            }
            else
            {
                dev.is_mini = true;
            }
        }
        rlist.push_back(dev);
    }
    return rlist;
}

/*--------------------------------------------------------------------------
 * Function:
 *     Limey_Device_List 
 */
Limey_Device_List::Limey_Device_List( )
{

}

/*--------------------------------------------------------------------------
 * Function:
 *     ~Limey_Device_List 
 */
Limey_Device_List::~Limey_Device_List()
{
}


