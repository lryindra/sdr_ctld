/**-------------------------------------------------------------------------
 * @file logger.cpp
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
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <unistd.h> // for getpid 
#include <iostream> // for cerr, cout
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include "application/logger.h"


/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
bool Logger::m_logger_configured(false);
const char* Logger::m_app_name(NULL);
std::string Logger::m_filename("../log/logger.properties");
std::string Logger::m_log_category("");

/*--------------------------------------------------------------------------
 * Function:
 *    load_config
 */
void Logger::load_config(const char* app_name,std::string file_name)
{
    if(!m_logger_configured) 
    {
        // store the app_name 
        m_app_name = app_name;

        m_filename = file_name;
        // try to to open the log files
        try
        {
            log4cpp::PropertyConfigurator::configure(m_filename); 
        }
        catch( log4cpp::ConfigureFailure &e )
        {
            std::cerr << e.what() 
                << " [log4cpp::ConfigureFailure catched] while reading " 
                << m_filename << std::endl;
        
            exit(1);
        }

        // get all loggers
        std::vector<log4cpp::Category*> *loggers = log4cpp::Category::getCurrentCategories();
        // index all loggers
        std::vector<log4cpp::Category*>::iterator logger_iterator = loggers->begin();
        try
        {
            // Get name the name of the last logger according to the index
            for(;logger_iterator!=loggers->end();logger_iterator++)
            {
                m_log_category = (*logger_iterator)->getName();
            }
        }
        catch(...)
        {
            std::cerr  
                << " [log4cpp catched] while searching for getName() " 
                << m_filename << std::endl;
        
            exit(1);
        }
        // send message to console
        log4cpp::Category& root =  log4cpp::Category::getRoot();
        root.info("Starting %s with PID: %d",m_app_name,getpid());
        
        // verify this logger exists and print the name to root
        static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
        if(NULL != logger_sub1)
        {
            // inform which logger is going to be used for the remainder of 
            // this class 
            const char *cstr = (logger_sub1->getName()).c_str();
            root.info("Logger: %s",cstr);
            root.info("filename: %s",m_filename.c_str());
        }
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    reset_config
 */
void Logger::reset_config()
{
    // send message to console
    log4cpp::Category& root = log4cpp::Category::getRoot();
    root.info("Good Bye.");
    // fetch local singleton of logger
    std::vector<log4cpp::Category*> *loggers = log4cpp::Category::getCurrentCategories();
    std::vector<log4cpp::Category*>::iterator logger_iterator = loggers->begin();
    //remove all appenders from categories
    for(;logger_iterator!=loggers->end();logger_iterator++)
    {
        (*logger_iterator)->removeAllAppenders();
    }

    m_filename = std::string("");
    m_logger_configured = false;
}

/*--------------------------------------------------------------------------
 * Function:
 *    debug
 */
void Logger::debug(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->debug(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    info
 */
void Logger::info(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->info(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    notice
 */
void Logger::notice(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->notice(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    warn
 */
void Logger::warn(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->warn(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    error
 */
void Logger::error(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->error(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    crit
 */
void Logger::crit(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->crit(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    alert
 */
void Logger::alert(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->alert(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    emerg
 */
void Logger::emerg(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->emerg(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    fatal
 */
void Logger::fatal(const std::string &msg)
{
    static log4cpp::Category* logger_sub1 = log4cpp::Category::exists(m_log_category);
    if(NULL != logger_sub1)
    {
        logger_sub1->fatal(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *    Logger 
 */
Logger::Logger()
{
}


/*--------------------------------------------------------------------------
 * Function:
 *     ~Logger
 */
Logger::~Logger()
{
    // gracefull shutdown
    log4cpp::Category::shutdown();
}


