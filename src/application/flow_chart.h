/**-------------------------------------------------------------------------
 * @file flow_chart.h
 * @brief build the flow chart's top block from GNURadio
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
#ifndef __FLOW_CHART_H__
#define __FLOW_CHART_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "application/radio_config.h"
#include <vector>
#include <string>
#include <gnuradio/top_block.h>
#include "audio/alsa_source.h"
#include "audio/alsa_sink.h"
#include "sdr/limey_sink_c.h"
#include "sdr/limey_source_c.h"
#include "receivers/ssbrx.h"
#include "transmitters/ssbtx.h"

class Flow_Chart;

/** pointer to member function in Flow_Chart class */
typedef std::string (Flow_Chart::*Flow_Chart_fnc_ptr)(std::string);

class Flow_Chart
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    static constexpr double get_audio_rate();

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    /** @brief Constructor
     *
     * @param config - configuration of this radio
     * @param center_freq - default frequency for start up
     */
    Flow_Chart(Radio_Config &rconfig, double center_freq = 60000000 );
    
    /** @brief Deconstructor
     *
     */
    ~Flow_Chart();

    /** @brief start the flow chart
     *
     * @return Void.
     */
    void start( void );

    /** @brief stop the flow chart
     *
     * @return Void.
     */
    void stop( void );

    /** @brief listen pop's messages from cmd_queue and pushes the response
     *
     * @return Void.
     */
    void listen( void );

private:
    struct {
        double input_rate;
        double min_freq;
    } typedef input_rate_t;
    static const input_rate_t m_input_rate_lime[];
    static constexpr double set_input_rate(const double i);
    double get_input_rate(const Radio_Config::radio_enum_t sdr);
    double get_min_freq(const Radio_Config::radio_enum_t sdr);
    std::string range_list;

    enum {
        PTT_RX = 0,
        PTT_TX = 1,
        PTT_TX_mic = 2,
        PTT_TX_data = 3,
        PTT_SIZE = 4
    } typedef PTT_ENUM;
    Radio_Config m_rconfig;
    std::vector<Flow_Chart_fnc_ptr> m_list;
    PTT_ENUM m_ptt;
    std::string m_vfo;

    gr::top_block_sptr m_top_block;
    gr::audio::alsa_source_sptr m_alsa_source;
    gr::audio::alsa_sink_sptr m_alsa_sink;
    Limey_Source_c::sptr m_sdr_source;
    Limey_Sink_c::sptr m_sdr_sink;
    ssbrx::sptr m_receiver;
    ssbtx::sptr m_transmitter;

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_set_freq(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_get_freq(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_set_mode(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_get_mode(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_set_vfo(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_get_vfo(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_set_ptt(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_get_ptt(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_check_vfo(std::string cmd);

    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_dump_state(std::string cmd);
    
    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_dump_caps(std::string cmd);
    
    /** @brief 
     *
     * @param std::string 
     * @return std::string 
     */
    std::string cmd_status(std::string cmd);

};

#endif /* __FLOW_CHART_H__ */
