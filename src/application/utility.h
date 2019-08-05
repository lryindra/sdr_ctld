/**-------------------------------------------------------------------------
 * @file utility.h
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
* --------------------------------------------------------------------------
*/
#ifndef __UTILITY_H__
#define __UTILITY_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <vector>
#include <string>
#include <iostream>

class Utility
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    static const std::string INVALID_PARAM;

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    
    /** @brief Print help information and exit.
     *
     * Called when main parses the input options.  This function prints the 
     * usage/help information to ostream. This function will not return, but 
     * will exit the program.
     *
     * @param std::ostream& pOstream - the pipe for the print output
     * @param const char* app_name - the name of this application
     * @return Void.
     */
    static void print_usage (std::ostream& pOstream, const char* app_name);

    /** @brief convert a string to an unsigned int
     *
     * @param std::string input
     * @param unsigned int * - return the unsigned int value
     * @param std::string * - the error string
     * @return bool - true if convert succeeded, else false
     */
    static bool stoui(std::string input, unsigned int *rval, std::string *error);

    /** @brief convert a string to a double
     *
     * @param std::string input
     * @param double * - return the double value
     * @param std::string * - the error string
     * @return bool - true if convert succeeded, else false
     */
    static bool stod(std::string input, double *rval, std::string *error);

    /** @brief convert a string to a float
     *
     * @param std::string input
     * @param float * - return the float value
     * @param std::string * - the error string
     * @return bool - true if convert succeeded, else false
     */
    static bool stof(std::string input, float *rval, std::string *error);

    /** @brief return the sub-string between the start string and end string
     *
     * @param input - the input string
     * @param start - the char before the substring
     * @param end - the char after the substring
     * @return std::string - returns "" if sub-string not found
     */
    static std::string get_substring(std::string input, const char start, const char end);
    static std::string get_substring(std::string input, std::string start, std::string end);

private:
    /** @brief Constructor
     *
     * @param Void.
     */
    Utility();

    /** @brief Deconstructor
     *
     * @param Void.
     */
    ~Utility();

};

#endif /* __UTILITY_H__ */
