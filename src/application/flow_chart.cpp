/**------------------------------------------------------------------------
 * @file flow_chart.cpp
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
 * ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "application/flow_chart.h"
#include "application/command_msg.h"
#include "application/logger.h"
#include "application/utility.h"
#include "application/message_queue.h"
#include <stdio.h>

/*-------------------------------------------------------------------------
 * Type Definitions
 * ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Function:
 *     get_audio_rate
 */
constexpr double Flow_Chart::get_audio_rate()
{
    return 48000;
}
/*-------------------------------------------------------------------------
 * Function:
 *     set_input_rate
 */
constexpr double Flow_Chart::set_input_rate(const double i)
{
    // the i here gets factored into two filters in transmitters/ssbtx.cpp
    return i*get_audio_rate();
}

// frequencies bellow this caused a seg fault
const Flow_Chart::input_rate_t Flow_Chart::m_input_rate_lime[] = 
 { { Flow_Chart::set_input_rate(27), 10000000 },  // for 30m on LimeSDR-Mini
   { Flow_Chart::set_input_rate(32), 6200000 } }; // for 40m on LimeSDR-USB
/*-------------------------------------------------------------------------
 * Function:
 *     get_input_rate
 */
double Flow_Chart::get_input_rate(const Radio_Config::radio_enum_t sdr)
{
    double rval = 0;
    switch(sdr)
    {
        case Radio_Config::LIMESDR_MINI:
            rval = m_input_rate_lime[0].input_rate;
            break;
        case Radio_Config::LIMSDR_USB:
            rval = m_input_rate_lime[1].input_rate;
            break;
        default:
            rval = set_input_rate(10);
            break;
    }
    return rval;
}
/*-------------------------------------------------------------------------
 * Function:
 *     get_min_freq
 */
double Flow_Chart::get_min_freq(const Radio_Config::radio_enum_t sdr)
{
    double rval = 0;
    switch(sdr)
    {
        case Radio_Config::LIMESDR_MINI:
            rval = m_input_rate_lime[0].min_freq;
            break;
        case Radio_Config::LIMSDR_USB:
            rval = m_input_rate_lime[1].min_freq;
            break;
        default:
            rval = 30000000;
            break;
    }
    return rval;
}

/*-------------------------------------------------------------------------
 * Function:
 *     Flow_Chart
 */
Flow_Chart::Flow_Chart(Radio_Config &rconfig, double center_freq )
{
    //load up the list of functon pointers
    //TODO: keep this updto date with Command_Msg::list
    m_list.push_back(&Flow_Chart::cmd_dump_caps);
    m_list.push_back(&Flow_Chart::cmd_set_freq);
    m_list.push_back(&Flow_Chart::cmd_get_freq);
    m_list.push_back(&Flow_Chart::cmd_set_mode);
    m_list.push_back(&Flow_Chart::cmd_get_mode);
    m_list.push_back(&Flow_Chart::cmd_set_vfo);
    m_list.push_back(&Flow_Chart::cmd_get_vfo);
    m_list.push_back(&Flow_Chart::cmd_set_ptt);
    m_list.push_back(&Flow_Chart::cmd_get_ptt);
    m_list.push_back(&Flow_Chart::cmd_check_vfo);
    m_list.push_back(&Flow_Chart::cmd_dump_state);
    m_list.push_back(&Flow_Chart::cmd_status);

    m_rconfig = rconfig;
    // initialize member variables
    m_ptt = PTT_RX;
    m_vfo = "VFO";

    // sound pointers
    std::string program_name = m_rconfig.get_program_name();
    m_alsa_sink = gnuradio::get_initial_sptr(new gr::audio::alsa_sink(get_audio_rate(), m_rconfig.get_sound_output_alsa(), true ));
    m_alsa_source = gnuradio::get_initial_sptr(new gr::audio::alsa_source(get_audio_rate(), m_rconfig.get_sound_input_alsa(), true ));

    // SDR pointers
    std::string serial = rconfig.get_sdr().serial; 
    double input_rate = get_input_rate(m_rconfig.get_sdr_type());
    double min_freq = get_min_freq(m_rconfig.get_sdr_type());
    if(center_freq < min_freq)
    {
        center_freq = Limey_Device_List::oscillator;
    }
    m_sdr_source = Limey_Source_c::make( serial, center_freq, input_rate, min_freq );
    m_sdr_sink = Limey_Sink_c::make( serial, center_freq, input_rate, min_freq );

    // receiver
    m_receiver = ssbrx::make(input_rate, get_audio_rate());
    // transmitter
    m_transmitter = ssbtx::make(input_rate, get_audio_rate());

    // create the range list for receive and transmit
    // mode information is from include/hamlib/rig.h
    unsigned long long int rig_mode_usb = 1ull << 2; 
    unsigned long long int rig_mode_lsb = 1ull << 3; 
    unsigned long long int rig_mode_pktusb = 1ull << 11; 
    unsigned long long int mode = rig_mode_usb | rig_mode_lsb | rig_mode_pktusb ;
    double max_freq = 3000000000; // 3 GHz
    int low_power = -1;
    int high_power = -1;
    std::string vfo = "10000003";
    std::string ant = "3";
    // start end modes low_power high_power vfo ant
    // %lf %lf %lld %d %d 0x%s 0x%s\n
    const int buf_size = 120;
    char range_list_char[buf_size];
    snprintf(range_list_char, buf_size, "%lf %lf %lld %d %d 0x%s 0x%s", min_freq, max_freq, mode, low_power, high_power, vfo.c_str(), ant.c_str());
    std::string range_list_str(range_list_char);
    // ex: "1296000.000000 3000000000.000000  2060 -1 -1 0x10000003 0x3";
    range_list = range_list_str;
}


/*-------------------------------------------------------------------------
 * Function:
 *    ~Flow_Chart
 */
Flow_Chart::~Flow_Chart()
{
}

/*-------------------------------------------------------------------------
 * Function:
 *     start
 */
void Flow_Chart::start( void )
{
    // connect all
    m_top_block = gr::make_top_block("top_block");
    try
    {
        m_top_block->connect( m_sdr_source, 0, m_receiver, 0);
        m_top_block->connect( m_receiver, 0, m_alsa_sink, 0);

        m_top_block->connect( m_alsa_source, 0, m_transmitter, 0);
        m_top_block->connect( m_transmitter, 0, m_sdr_sink, 0);
    }
    catch(std::invalid_argument& e)
    {
        Logger::crit("[Flow_Chart::Flow_Chart] You are trying to connect blocks with different sized ports (eg. connecting a complex to a float). "+(std::string(e.what())));
        throw std::string(e.what());
    }

    m_top_block->start();
}

/*-------------------------------------------------------------------------
 * Function:
 *     stop
 */
void Flow_Chart::stop( void )
{
    if( nullptr != m_top_block )
    {
        m_top_block->stop();
        m_top_block->wait();

        m_top_block->disconnect( m_sdr_source, 0, m_receiver, 0);
        m_top_block->disconnect( m_receiver, 0, m_alsa_sink, 0);

        m_top_block->disconnect( m_alsa_source, 0, m_transmitter, 0);
        m_top_block->disconnect( m_transmitter, 0, m_sdr_sink, 0);

        m_top_block = nullptr;
    }
}

/*-------------------------------------------------------------------------
 * Function:
 *     listen
 */
void Flow_Chart::listen( void )
{
    Message_Queue::sptr cmd_queue = m_rconfig.get_cmd_queue();
    Message_Queue::sptr rsp_queue = m_rconfig.get_rsp_queue();
    while( !cmd_queue->empty() )
    {
        std::string param = "";
        Message_Queue::message_t msg_struct = cmd_queue->pop_struct();
        std::string msg = msg_struct.msg;
        // get the index
        int index = Command_Msg::get_index(msg);
        std::size_t list_size = m_list.size();
        if(-1 < index && index < (int)list_size)
        {
            Flow_Chart_fnc_ptr cmd_fnc = m_list[index];
            std::string rsp = (this->*cmd_fnc)(msg);
            Message_Queue::message_t rsp_struct;
            rsp_struct.msg = rsp;
            rsp_struct.fd = msg_struct.fd;
            rsp_queue->push(rsp_struct);
        }
        else
        {
            Logger::warn("[Flow_Chart::listen] This command does not have a matching function pointer "+msg);
            Message_Queue::message_t rsp_struct;
            rsp_struct.msg = Command_Msg::append_delim("RPRT -4");
            rsp_struct.fd = msg_struct.fd;
            rsp_queue->push(rsp_struct);
        }
    }
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_set_freq
 */
std::string Flow_Chart::cmd_set_freq(std::string cmd)
{
    std::string rval;
    unsigned int freq = 0;
    // parse cmd
    std::string param = Utility::get_substring(cmd, Command_Msg::space, Command_Msg::delim);
    // rval is set if stoui is false
    // freq is set if stoui is true
    if(Utility::stoui(param,&freq,&rval))
    {
        Logger::debug("[Flow_Chart::cmd_set_freq] frequency="+std::to_string(freq));
        // TODO: check if(m_ptt == PTT_RX) before allowed to change freq
        bool source = m_sdr_source->set_center_frequency(freq);
        bool sink = m_sdr_sink->set_center_frequency(freq);
        if( !(source && sink) )
        {
            Logger::notice("There was a error chaning the frequency.");
        }
        rval = Command_Msg::append_delim("RPRT 0");
    }
    return rval;
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_get_freq
 */
std::string Flow_Chart::cmd_get_freq(std::string cmd)
{
    std::string rval = "";
    double source = m_sdr_source->get_center_frequency();
    double sink = m_sdr_sink->get_center_frequency();
    if(source == sink)
    {
        rval = std::to_string(source);
    }
    else
    {
        rval = std::to_string(source)+" "+std::to_string(sink);
    }
    return (Command_Msg::append_delim(rval));
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_set_mode
 */
std::string Flow_Chart::cmd_set_mode(std::string cmd)
{
    std::string param = Utility::get_substring(cmd, Command_Msg::space, Command_Msg::delim);
    Logger::debug("[Flow_Chart::cmd_set_mode] mode="+param);
    return (Command_Msg::append_delim("RPRT 0"));
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_get_mode
 */
std::string Flow_Chart::cmd_get_mode(std::string cmd)
{
    return (Command_Msg::append_delim("PKTUSB"));
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_set_vfo
 */
std::string Flow_Chart::cmd_set_vfo(std::string cmd)
{
    const std::vector<std::string> vfo_list = 
        { "VFOA",
          "VFOB",
          "VFOC",
          "currVFO",
          "VFO",
          "MEM",
          "Main",
          "Sub",
          "TX",
          "RX" };
    std::string param = Utility::get_substring(cmd, Command_Msg::space, Command_Msg::delim);
    Logger::debug("[Flow_Chart::cmd_set_vfo] vfo="+param);
    for(unsigned int i = 0; i < vfo_list.size(); i++)
    {
        if(param == vfo_list[i])
        {
            m_vfo = vfo_list[i];
            return (Command_Msg::append_delim("RPRT 0"));
        }
    }
    return (Command_Msg::append_delim("RPRT -9"));
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_get_vfo
 */
std::string Flow_Chart::cmd_get_vfo(std::string cmd)
{
    return (Command_Msg::append_delim(m_vfo));
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_get_mode
 */
std::string Flow_Chart::cmd_set_ptt(std::string cmd)
{
    std::string rval;
    unsigned int mode = PTT_SIZE;
    // parse cmd
    std::string param = Utility::get_substring(cmd, Command_Msg::space, Command_Msg::delim);
    if(Utility::stoui(param,&mode,&rval))
    {
        if(PTT_SIZE > mode)
        {
            Logger::debug("[Flow_Chart::cmd_set_ptt] ptt="+std::to_string(mode));
            m_ptt = (PTT_ENUM)(mode);
            if(mode == PTT_RX)
            {
                m_transmitter->ptt_off();
            }
            else
            {
                m_transmitter->ptt_on();
            }
            rval = Command_Msg::append_delim("RPRT 0");
        }
        else
        {
            rval = Utility::INVALID_PARAM;
        }
    }
    return rval;
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_get_mode
 */
std::string Flow_Chart::cmd_get_ptt(std::string cmd)
{
    unsigned int ptt_mode = (unsigned int) m_ptt;
    std::string rval (Command_Msg::append_delim(ptt_mode));
    return rval;
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_check_vfo
 */
std::string Flow_Chart::cmd_check_vfo(std::string cmd)
{
    std::string rval (Command_Msg::append_delim("CHKVFO 0"));
    return rval;
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_dump_state(
 */
std::string Flow_Chart::cmd_dump_state(std::string cmd)
{
    std::vector<std::string> state = 
                       { "0", // always 0
                         "2", // %d rig->caps->rig_model Hamlib NET rigctl
                         "2", // %d rs->itu_region RIG_ITU_REGION2
                         // repeat the following for each in rs->rx_range_list
                         range_list,
                         // delimeter
                         "0 0 0 0 0 0 0",
                         // repeat the following for each in rs->tx_range_list
                         range_list,
                         // delimeter
                         "0 0 0 0 0 0 0",
                         // repeat the following for each in rs->tuning_steps
                         // modes ts
                         // delimeter
                         "0 0",
                         // repeat the following for each in rs->filters
                         // modes width
                         // delimeter
                         "0 0",
                         // rs->max_rit
                         "0",
                         // rs->max_xit
                         "0",
                         // rs->max_ifshift
                         "0",
                         // rx->announces
                         "0",
                         // repeat the following for each rs->preamp
                         // rs->preamp
                         // delimeter
                         "",
                         "",
                         "0x0", // rs->has_get_func
                         "0x0", // rs->has_set_func
                         "0x0", // rs->has_get_level
                         "0x0", // rs->has_get_parm
                         "0x0", // rs->has_set_parm
                         "0" };
    std::string rval = "";
    for( unsigned int i = 0; i < state.size(); i++ )
    {
        rval += Command_Msg::append_delim(state[i]);
    }
    Logger::debug("[Flow_Chart::cmd_dump_state] dump state.");
    return rval;
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_dump_caps
 */
std::string Flow_Chart::cmd_dump_caps(std::string cmd)
{
    std::vector<std::string> rlist = 
                     { "Caps dump for model: lime",
                       "Model name:	LimeSDR Mini",
                       "Mfg name:	Myriadrf",
                       "Backend version:	0.0",
                       "Backend copyright:	GPL ver. 3",
                       "Backend status:	Beta",
                       "Rig type:	Transceiver",
                       "PTT type:	Rig capable",
                       "DCD type:	Rig capable",
                       "Port type:	Network link",
                       "Write delay: 0mS, timeout 2000mS, 3 retry",
                       "Post Write delay: 0mS",
                       "Has targetable VFO: Y",
                       "Has transceive: N",
                       "Announce: 0x0",
                       "Max RIT: -0.0kHz/+0.0kHz",
                       "Max XIT: -0.0kHz/+0.0kHz",
                       "Max IF-SHIFT: -0.0kHz/+0.0kHz",
                       "Preamp: None",
                       "Attenuator: None",
                       "CTCSS: None",
                       "DCS: None",
                       "Get functions: ",
                       "Set functions: ",
                       "Get level: ",
                       "Set level: ",
                       "Extra levels:",
                       "Get parameters: ",
                       "Set parameters: ",
                       "Extra parameters:",
                       "Mode list: PKTUSB",
                       "",
                       "VFO list: "+m_vfo,
                       "",
                       "VFO Ops: ",
                       "Scan Ops: ",
                       "Number of banks:	0",
                       "Memory name desc size:	0",
                       "Memories: None",
                       "TX ranges status, region 1:	OK (0)",
                       "RX ranges status, region 1:	OK (0)",
                       "TX ranges status, region 2:	OK (0)",
                       "RX ranges status, region 2:	OK (0)",
                       "Tuning steps: ",
                       "        1 Hz:           PKTUSB",
                       "Tuning steps status:	OK (0)",
                       "Filters: ",
                       "       2.2 kHz:          PKTUSB",
                       "Bandwidths:",
                       "       PKTUSB   Normal: 2.2 kHz, Narrow: 0 Hz, Wide: 0 Hz ",
                       "Has priv data:	N",
                       "Has Init:	Y",
                       "Has Cleanup:	Y",
                       "Has Open:	Y",
                       "Has Close:	Y",
                       "Can set Conf:	N",
                       "Can get Conf:	N",
                       "Can set Frequency:	Y",
                       "Can get Frequency:	Y",
                       "Can set Mode:	Y",
                       "Can get Mode:	Y",
                       "Can set VFO:	Y",
                       "Can get VFO:	Y",
                       "Can set PTT:	N",
                       "Can get PTT:	N",
                       "Can get DCD:	N",
                       "Can set Repeater Duplex:	N",
                       "Can get Repeater Duplex:	N",
                       "Can set Repeater Offset:	N",
                       "Can get Repeater Offset:	N",
                       "Can set Split Freq:	N",
                       "Can get Split Freq:	N",
                       "Can set Split Mode:	N",
                       "Can get Split Mode:	N",
                       "Can set Split VFO:	N",
                       "Can get Split VFO:	N",
                       "Can set Tuning Step:	N",
                       "Can get Tuning Step:	N",
                       "Can set RIT:	N",
                       "Can get RIT:	N",
                       "Can set XIT:	N",
                       "Can get XIT:	N",
                       "Can set CTCSS:	N",
                       "Can get CTCSS:	N",
                       "Can set DCS:	N",
                       "Can get DCS:	N",
                       "Can set CTCSS Squelch:	N",
                       "Can get CTCSS Squelch:	N",
                       "Can set DCS Squelch:	N",
                       "Can get DCS Squelch:	N",
                       "Can set Power Stat:	N",
                       "Can get Power Stat:	N",
                       "Can Reset:	N",
                       "Can get Ant:	N",
                       "Can set Ant:	N",
                       "Can set Transceive:	N",
                       "Can get Transceive:	N",
                       "Can set Func:	N",
                       "Can get Func:	N",
                       "Can set Level:	N",
                       "Can get Level:	N",
                       "Can set Param:	N",
                       "Can get Param:	N",
                       "Can send DTMF:	N",
                       "Can recv DTMF:	N",
                       "Can send Morse:	N",
                       "Can decode Events:	N",
                       "Can set Bank:	N",
                       "Can set Mem:	N",
                       "Can get Mem:	N",
                       "Can set Channel:	N",
                       "Can get Channel:	N",
                       "Can ctl Mem/VFO:	N",
                       "Can Scan:	N",
                       "Can get Info:	N",
                       "Can get power2mW:	N",
                       "Can get mW2power:	N",
                       "",
                       "Overall backend warnings: 0",
                       "RPRT 0"};
    std::string rval = "";
    for( unsigned int i = 0; i < rlist.size(); i++ )
    {
        rval += Command_Msg::append_delim(rlist[i]);
    }
    return rval;
}

/*-------------------------------------------------------------------------
 * Function:
 *     cmd_status
 */
std::string Flow_Chart::cmd_status(std::string cmd)
{
    Logger::debug("[Flow_Chart::cmd_status] '"+cmd+"' ");

    return cmd;
}

