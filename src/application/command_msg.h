/**-------------------------------------------------------------------------
 * @file command_msg.h
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
 * -------------------------------------------------------------------------
 */
#ifndef __COMMAND_MSG_H__
#define __COMMAND_MSG_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <string>
#include <vector>

class Command_Msg
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    /** all incoming messages are expected to contain this delimiter */
    static const char delim = '\n';
    static const char space = ' ';

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    /** @brief 
     *
     * @param cmd
     * @return size_t
     */
    static int get_index(std::string cmd);

    /** @brief return true if this string is a known message
     *
     * @param cmd
     * @return bool
     */
    static bool check(std::string cmd);

    /** @brief append the delim to the end of the string
     *
     * @param std::string
     * @return std::string
     */
    static std::string append_delim(std::string rsp);
    static std::string append_delim(unsigned int rsp);

private:
    static const std::vector<std::string> short_list;
    static const std::vector<std::string> long_list;

    /** @brief Constructor
     *
     * @param Void.
     */
    Command_Msg();

    /** @brief Deconstructor
     *
     * @param Void.
     */
    ~Command_Msg();

};

#endif /* __COMMAND_MSG_H__ */
