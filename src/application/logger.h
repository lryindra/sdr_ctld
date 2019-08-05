/**-------------------------------------------------------------------------
 * @file logger.h
 * @brief Create and access to a debugger's log
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
#ifndef _LOGGER_H
#define _LOGGER_H

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <string>

class Logger {
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/

    /** @brief Method to load configuration
     *
     * Load the configuration with the default parameters
     *
     * @param const char* app_name - name of application
     * @param std::string path to the properties file
     * @return Void.
     */
    static void load_config(const char* app_name,std::string file_name);

    /** @brief Method to reset logger configuration
     *
     * @param Void.
     * @return Void.
     */
    static void reset_config();

    /** @brief Log debug message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void debug(const std::string &msg);

    /** @brief Log info message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void info(const std::string &msg);

    /** @brief Log notice message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void notice(const std::string &msg);

    /** @brief Log warn message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void warn(const std::string &msg);

    /** @brief Log error message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void error(const std::string &msg);

    /** @brief Log critical message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void crit(const std::string &msg);

    /** @brief Log alert message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void alert(const std::string &msg);

    /** @brief Log emerg message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void emerg(const std::string &msg);

    /** @brief Log fatal message
     *
     * @param const std::string &msg
     * @return Void.
     */
    static void fatal(const std::string &msg);

private:
    static bool m_logger_configured;
    static const char* m_app_name;
    static std::string m_filename;
    static std::string m_log_category;

    /** @brief Constructor
     *
     * @param Void.
     */
    Logger();

    /** @brief Deconstructor
     *
     * @param Void.
     */
    ~Logger();

}; /* end class */
#endif /*  _LOGGER_H */
