/**------------------------------------------------------------------------
 * @file radio_config.cpp
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
 * ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "application/radio_config.h"

/*-------------------------------------------------------------------------
 * Type Definitions
 * ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Function:
 *     Radio_Config
 */
Radio_Config::Radio_Config()
{
}

/*-------------------------------------------------------------------------
 * Function:
 *    ~Radio_Config
 */
Radio_Config::~Radio_Config()
{
}

/*-------------------------------------------------------------------------
 * Function:
 *     get_program_name
 */
std::string Radio_Config::get_program_name()
{
    return m_program_name;
}

/*-------------------------------------------------------------------------
 * Function:
 *     set_program_name
 */
void Radio_Config::set_program_name(std::string name)
{
    m_program_name = name;
}
void Radio_Config::set_program_name(const char* name)
{
    std::string name_str(name);
    size_t idx = name_str.rfind("/");
    if(idx != std::string::npos)
    {
        idx++;
        size_t name_len = name_str.size() - idx;
        m_program_name = name_str.substr(idx,name_len);
    }
}

/*-------------------------------------------------------------------------
 * Function:
 *     get_sound_input_alsa
 */
std::string Radio_Config::get_sound_input_alsa()
{
    return m_sound_input_alsa;
}

/*-------------------------------------------------------------------------
 * Function:
 *     set_sound_input_alsa
 */
void Radio_Config::set_sound_input_alsa(std::string snd)
{
    m_sound_input_alsa = snd;
}

/*-------------------------------------------------------------------------
 * Function:
 *     get_sound_output_alsa
 */
std::string Radio_Config::get_sound_output_alsa()
{
    return m_sound_output_alsa;
}

/*-------------------------------------------------------------------------
 * Function:
 *     set_sound_output_alsa
 */
void Radio_Config::set_sound_output_alsa(std::string snd)
{
    m_sound_output_alsa = snd;
}

/*-------------------------------------------------------------------------
 * Function:
 *     get_sdr_type
 */
Radio_Config::radio_enum_t Radio_Config::get_sdr_type()
{
    radio_enum_t sdr;
    if(m_sdr_dev.is_mini)
    {
        sdr = LIMESDR_MINI;
    }
    else
    {
        sdr = LIMSDR_USB;
    }
    return sdr;
}

/*-------------------------------------------------------------------------
 * Function:
 *     get_sdr
 */
Limey_Device_List::limey_device_t Radio_Config::get_sdr()
{
    return m_sdr_dev;
}

/*-------------------------------------------------------------------------
 * Function:
 *     set_sdr
 */
void Radio_Config::set_sdr(Limey_Device_List::limey_device_t dev)
{
    m_sdr_dev = dev;
}

/*-------------------------------------------------------------------------
 * Function:
 *     get_cmd_queue
 */
Message_Queue::sptr Radio_Config::get_cmd_queue()
{
    return m_cmd_queue;
}

/*-------------------------------------------------------------------------
 * Function:
 *     set_cmd_queue
 */
void Radio_Config::set_cmd_queue(Message_Queue::sptr queue)
{
    m_cmd_queue = queue;
}

/*-------------------------------------------------------------------------
 * Function:
 *     get_rsp_queue
 */
Message_Queue::sptr Radio_Config::get_rsp_queue()
{
    return m_rsp_queue;
}

/*-------------------------------------------------------------------------
 * Function:
 *     set_rsp_queue
 */
void Radio_Config::set_rsp_queue(Message_Queue::sptr queue)
{
    m_rsp_queue = queue;
}

