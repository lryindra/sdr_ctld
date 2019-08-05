/**-------------------------------------------------------------------------
 * @file radio_config.h
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
 * -----------------------------------------------------------------------*/
#ifndef __RADIO_CONFIG_H__
#define __RADIO_CONFIG_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "application/message_queue.h"
#include "sdr/limey_device_list.h"
#include <string>

class Radio_Config
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    enum {
        LIMESDR_MINI,
        LIMSDR_USB
    } typedef radio_enum_t;

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    /** @brief Constructor
     *
     * @param Void.
     */
    Radio_Config();
    
    /** @brief Deconstructor
     *
     */
    ~Radio_Config();

    /** @brief Get program name
     *
     * @return std::string
     */
    std::string get_program_name();

    /** @brief Set program name
     *
     * @param name - program name
     * @return Void.
     */
    void set_program_name(std::string name);
    void set_program_name(const char* name);

    /** @brief get the name of the sound input ALSA device
     *
     * @return std::string
     */
    std::string get_sound_input_alsa();

    /** @brief set the name of the sound input ALSA device
     *
     * @param snd - input sound
     * @return Void.
     */
    void set_sound_input_alsa(std::string snd);

    /** @brief get the name of the sound output ALSA device
     *
     * @return std::string
     */
    std::string get_sound_output_alsa();

    /** @brief set the name of the sound output ALSA device
     *
     * @param snd - input sound
     * @return Void.
     */
    void set_sound_output_alsa(std::string snd);

    /** @brief Get the SDR specified by radio_enum_t
     *
     * @return radio_enum_t
     */
    radio_enum_t get_sdr_type();

    /** @brief Get LimeSDR device
     *
     * @return Limey_Device_List::limey_device_t 
     */
    Limey_Device_List::limey_device_t get_sdr();

    /** @brief Set LimeSDR device
     *
     * @param dev - SDR device
     * @return Void.
     */
    void set_sdr(Limey_Device_List::limey_device_t dev);

    /** @brief Get command queue
     *
     * @return std::string
     */
    Message_Queue::sptr get_cmd_queue();

    /** @brief Set command queue
     *
     * @param queue - command queue pointer
     * @return Void.
     */
    void set_cmd_queue(Message_Queue::sptr queue);

    /** @brief Get response queue
     *
     * @return std::string
     */
    Message_Queue::sptr get_rsp_queue();

    /** @brief Set response queue
     *
     * @param queue - response queue pointer
     * @return Void.
     */
    void set_rsp_queue(Message_Queue::sptr queue);

private:
    Message_Queue::sptr m_cmd_queue;
    Message_Queue::sptr m_rsp_queue;
    std::string m_program_name;
    std::string m_sound_input_alsa;
    std::string m_sound_output_alsa;
    std::string m_sdr_name;
    Limey_Device_List::limey_device_t m_sdr_dev;

};

#endif /* __RADIO_CONFIG_H__ */
