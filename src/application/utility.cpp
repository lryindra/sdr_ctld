/**-------------------------------------------------------------------------
 * @file utility.cpp
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
#include "application/utility.h"
#include <stdexcept>
#include <cstdlib>
#include <cctype> //isalnum

/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
const std::string Utility::INVALID_PARAM =  "RPRT -9\n";

/*--------------------------------------------------------------------------
 * Function:
 *     Utility
 */
void Utility::print_usage (std::ostream& pOstream, const char* app_name)
{
    pOstream << "Usage: " << app_name << " [ options ]"<< std::endl;
    pOstream << "  -h --help                  Display this usage information.\n"
        << "  -f --freq [center freq.]   Default center frequency in Hz.\n"
        << "  -o --snd-out [name]        Name of the audio output.\n"
        << "  -i --snd-in [name]         Name of the audio input.\n"
        << "  -s --sel-sdr [name]        Select the SDR from the list of SDRs.\n"
        << "  -l --list-sdr              Print the available SDRs and exit.\n"
        << "  -t --tcp-port [portNumber] The TCP port number." <<std::endl;
}

/*--------------------------------------------------------------------------
 * Function:
 *     stoui
 */
bool Utility::stoui(std::string input, unsigned int *rval, std::string *error)
{
    int i;
    try
    {
        i = std::stoi(input);
    }
    catch (std::invalid_argument& e)
    {
        *error = INVALID_PARAM;
        return false;
    }
    catch (std::out_of_range& e)
    {
        *error = INVALID_PARAM;
        return false;
    }
    if( 0 > i )
    {
        *error = INVALID_PARAM;
        return false;
    }
    *rval = i;
    return true;
}

/*--------------------------------------------------------------------------
 * Function:
 *     stod
 */
bool Utility::stod(std::string input, double *rval, std::string *error)
{
    double i;
    try
    {
        i = std::stod(input);
    }
    catch (std::invalid_argument& e)
    {
        *error = INVALID_PARAM;
        return false;
    }
    catch (std::out_of_range& e)
    {
        *error = INVALID_PARAM;
        return false;
    }
    *rval = i;
    return true;
}

/*--------------------------------------------------------------------------
 * Function:
 *     stof
 */
bool Utility::stof(std::string input, float *rval, std::string *error)
{
    float i;
    try
    {
        i = std::stof(input);
    }
    catch (std::invalid_argument& e)
    {
        *error = INVALID_PARAM;
        return false;
    }
    catch (std::out_of_range& e)
    {
        *error = INVALID_PARAM;
        return false;
    }
    *rval = i;
    return true;
}

/*--------------------------------------------------------------------------
 * Function:
 *     get_substring
 */
std::string Utility::get_substring(std::string input, const char start, const char end)
{
    std::string rval = "";
    size_t idx = input.find(start);
    size_t idx_start = 1 + idx;
    if(idx == std::string::npos || idx_start >= input.size())
    {
        // Return with empty string because the sub-string was not found
    }
    else
    {
        std::string new_str = input.substr(idx_start,(input.size()-idx_start));
        size_t idx_end = new_str.find(end);
        if(idx_end == std::string::npos)
        {
            idx_end = new_str.size();
        }
        rval = new_str.substr(0,idx_end);
    }
    return rval;
}
std::string Utility::get_substring(std::string input, std::string start, std::string end)
{
    std::string rval = "";
    size_t idx = input.find(start);
    size_t idx_start = start.size() + idx;
    if(idx == std::string::npos || idx_start >= input.size())
    {
        // Return with empty string because the sub-string was not found
    }
    else
    {
        std::string new_str = input.substr(idx_start,(input.size()-idx_start));
        size_t idx_end = new_str.find(end);
        if(idx_end == std::string::npos)
        {
            idx_end = new_str.size();
        }
        rval = new_str.substr(0,idx_end);
    }
    return rval;
}

/*--------------------------------------------------------------------------
 * Function:
 *     Utility
 */
Utility::Utility()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~Utility
 */
Utility::~Utility()
{
}


