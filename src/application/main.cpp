/**-------------------------------------------------------------------------
 * @file main.cpp
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
 *----------------------------------------------------------------------- */
#include <iostream>
#include <getopt.h>
#include <stdlib.h> // strtoul
#include <string>
#include "sdr/limey_device_list.h"
#include "application/radio_config.h"
#include "application/logger.h"
#include "application/utility.h"
#include "application/message_queue.h"
#include "application/message_server.h"
#include "application/flow_chart.h"
#include <thread>
#include <future>
#include <chrono>

/** @brief non-blocking cin 
 *
 * scan the standard input and return the result
 *
 * @return std::string 
 */
static std::string nonblock_cin()
{
    std::string cmd;
    std::cin >> cmd;
    return cmd;
}

/*------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
    int next_option;
    // pulse audio list
    std::string snd_in_idx = "";
    std::string snd_out_idx = "";

    // SDR device 
    std::string sdr_serial_str = "";

    // default TCP port number
    int port_num = 4532;
    // default center frequency
    unsigned long center_freq  = 50293000;
    // the name of this program
    const char* program_name = argv[0];
    // A string listing valid short options letters.
    const char* const short_options = "ht:lo:i:s:f:";
    // An array describing valid long options
    const struct option long_options[] = {
        { "help",       0, NULL, 'h' },
        { "freq",       0, NULL, 'f' }, //-f --freq 
        { "sel-sdr",    0, NULL, 's' },
        { "snd-in",     0, NULL, 'i' },
        { "snd-out",    0, NULL, 'o' },
        { "list-sdr",   0, NULL, 'l' },
        { "tcp-port",   0, NULL, 't'},
        { NULL,         0, NULL, 0 } // Required at end of array
    };

    // Parse the command line options
    do {
        next_option = getopt_long (argc, argv, short_options, long_options, NULL);
        switch(next_option)
        {
        case 'f': // -f or --freq
                center_freq = strtoul(optarg,NULL,0);
                break;
        case 's': // -s or --sel-sdr
                sdr_serial_str  = std::string(optarg);
                break;
        case 'i': // -i or --snd-in
                snd_in_idx = std::string(optarg);
                break;
        case 'o':
                snd_out_idx = std::string(optarg);
                break;
        case 'l': // -l or --list-sdr
                std::cout << "Select one of the following devices: " << std::endl;
                for( Limey_Device_List::limey_device_t dev : Limey_Device_List::get_device_list())
                {
                    std::cout << dev.serial << std::endl;
                }
                std::exit(0);
                break;
        case 't': // -t or --tcp-port
                port_num = std::atoi(optarg);
                if(1024 > port_num || 49151 < port_num)
                {
                    std::cerr << "Port number "<< port_num << " is not valid. Please select a port number from 1024 to 49151."<< std::endl;
                    exit(1);
                }
                break;
        case 'h': // -h or --help
                // User requested usage information
                Utility::print_usage(std::cout, program_name);
                std::exit(0);
        case '?': // invalid option
                Utility::print_usage(std::cerr, program_name);
                std::exit(1);
        case -1: // done with options
                break;

        default: // something unexpected.
                std::cerr << "There was an error parsing options." << std::endl;
                exit(1);
        }
    } while (next_option != -1);

    // setup the logging 
    Logger::load_config(program_name,"../log/logger.properties");
    // configure
    Radio_Config rconfig;
    rconfig.set_program_name(program_name);
    // setup message queues
    rconfig.set_cmd_queue(Message_Queue::make());
    rconfig.set_rsp_queue(Message_Queue::make());
    // setup tcp server 
    Message_Server server( rconfig.get_cmd_queue(), rconfig.get_rsp_queue(), port_num);
    try
    {
        server.connect();
    }
    catch (const char* msg)
    {
        std::cout << "exiting because of a server error.\n";
        exit(1);
    }

    // Audio information
    rconfig.set_sound_input_alsa(snd_in_idx);
    Logger::info("Audio input: "+rconfig.get_sound_input_alsa());
    rconfig.set_sound_output_alsa(snd_out_idx);
    Logger::info("Audio output: "+rconfig.get_sound_output_alsa());
    // SDR information, Do this after setting up logging
    std::vector<Limey_Device_List::limey_device_t> sdr_list = Limey_Device_List::get_device_list();
    Limey_Device_List::limey_device_t sdr_dev;
    if("" == sdr_serial_str)
    {
        // if the sdr_serial_str was not specified with the command line 
        // option, i the pick the first device in the list
        sdr_serial_str = sdr_list[0].serial;
        sdr_dev = sdr_list[0];
    }
    else
    {
        unsigned int i = 0;
        for( i=0; i < sdr_list.size(); i++)
        {
            if(sdr_list[i].serial == sdr_serial_str)
            {
                sdr_dev = sdr_list[i];
                break;
            }
        }
        if(i == sdr_list.size())
        {
            Logger::notice("SDR not found: '"+sdr_serial_str+"'");
            //disable logger
            Logger::reset_config();
            exit(1);
        }
    }
    rconfig.set_sdr(sdr_dev);
    Logger::info("SDR: "+rconfig.get_sdr().info);
    // setup flow chart
    Flow_Chart fl( rconfig, center_freq );

    // start the SDR
    fl.start();
    // this bit captures input from the command line to exit the program
    std::future<std::string> fut = std::async(std::launch::async, nonblock_cin);
    std::cout << "Type q [return] to exit." << std::endl;
    std::future_status status;
    do 
    {
        // message server checks for new messages
        server.listen();
        // flow chart listens for messages
        fl.listen();
        // message server transmits responses
        server.send_response();
        // look for keyboard input from user
        status = fut.wait_for(std::chrono::milliseconds(100));
        if( status == std::future_status::deferred )
        {
            std::cout << "deferred\n";
        }
        //else if( status == std::future_status::timeout )
        //{
        //    std::cout << "timeout\n";
        //}
        else if( status == std::future_status::ready )
        {
            std::cout << "ready\n";
        }
    } while( status != std::future_status::ready );
    
    std::cout << "result is " << fut.get() << std::endl;

    // stop the SDR
    fl.stop();

    // close TCP connections
    server.disconnect();

    //disable logger
    Logger::reset_config();
    return 0;
}


