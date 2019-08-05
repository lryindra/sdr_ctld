/**-------------------------------------------------------------------------
 * @file command_msg.cpp
 * @brief do some message parsing of commang messages
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
 *------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "application/command_msg.h"

/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
const std::vector<std::string> Command_Msg::short_list ({
        "1",
        "F",
        "f",
        "M",
        "m",
        "V",
        "v",
        "T",
        "t",
        "0xf0",
        "0x8f",
        "q" });
// long commands are preceeded by '\'
const std::vector<std::string> Command_Msg::long_list ({
        "dump_caps",
        "set_freq",
        "get_freq",
        "set_mode",
        "get_mode",
        "set_vfo",
        "get_vfo",
        "set_ptt",
        "get_ptt",
        "chk_vfo",
        "dump_state",
        "quit" });

/*--------------------------------------------------------------------------
 * Function:
 *     Command_Msg
 */
Command_Msg::Command_Msg()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~Command_Msg
 */
Command_Msg::~Command_Msg()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_index
 */
int Command_Msg::get_index(std::string cmd)
{
    int index = -1;
    std::size_t pos_delim = cmd.find_first_of(delim); 
    if(pos_delim != std::string::npos)
    {
        std::string sub_cmd = "";
        // found the message 
        // next step is to isolate the command
        std::size_t pos_space = cmd.find_first_of(' '); 
        if(pos_space != std::string::npos)
        {
            // found a space
            // remove the paramter list
            sub_cmd = cmd.substr(0,pos_space);
        }
        else
        {
            // use the whole packet
            sub_cmd = cmd.substr(0,pos_delim);

        }
        if(0 < sub_cmd.size())
        {
            // don't match an empty string
            // determin if this is a long command or a short command
            if('\\' == sub_cmd[0])
            {
                // this is a long command
                // ignore the first char
                std::string long_cmd = sub_cmd.substr(1);
                // find the command in the list
                std::vector<int>::size_type i = 0;
                for(i = 0; i < long_list.size(); i++)
                {
                    if(0 == long_cmd.compare(long_list[i]))
                    {
                        index = i;
                        break;
                    }
                }
            }
            else
            {
                // this is a short command
                std::vector<int>::size_type i = 0;
                for(i = 0; i < short_list.size(); i++)
                {
                    if(0 == sub_cmd.compare(short_list[i]))
                    {
                        index = i;
                        break;
                    }
                }
            }
        }
    }
    return index;
}

/*--------------------------------------------------------------------------
 * Function:
 *     check
 */
bool Command_Msg::check(std::string cmd)
{
    bool rval = false;
    if(-1 < Command_Msg::get_index(cmd) )
    {
        rval = true;
    }
    return rval;
}

/*--------------------------------------------------------------------------
 * Function:
 *     append_delim
 */
std::string Command_Msg::append_delim(std::string rsp)
{
    const char delim_char = delim;
    std::string delim_str (&delim_char,1); 
    return (rsp+delim_str);
}
std::string Command_Msg::append_delim(unsigned int rsp)
{
    const char delim_char = delim;
    std::string delim_str (&delim_char,1);
    std::string rsp_str (std::to_string(rsp));
    return (rsp_str+delim_str);
}

