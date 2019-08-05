/**-------------------------------------------------------------------------
 * @file tcp_server.cpp 
 * @brief Open and listing to a TCP port
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
#include "application/tcp_server.h"
#include "application/logger.h"
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>

/*--------------------------------------------------------------------------
 * Function:
 *     TCP_Server
 */
TCP_Server::TCP_Server(Message_Queue::sptr msg_queue)
    : m_listening(false)
{
    m_msg_queue = msg_queue;
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~TCP_Server
 */
TCP_Server::~TCP_Server()
{
    disconnect();
}

/*--------------------------------------------------------------------------
 * Function:
 *     accept_conn
 */
void TCP_Server::accept_conn( void)
{
    int addrlen = sizeof(m_address);
    while(m_listening)
    {
        int sock = accept(m_server_fd, (struct sockaddr *)&m_address, (socklen_t*)&addrlen);
        if(0 <= sock)
        {
            m_socket.push_back(sock);
            Logger::debug("[TCP_Server::connect] accepted connection. ");
            // spin off a thread for accepting a connection
            std::thread t1(&TCP_Server::read_from_sock,this,sock);
            t1.detach();
        }
    }
    Logger::debug("[TCP_Server::accept_conn] exiting thread.");
}

/*--------------------------------------------------------------------------
 * Function:
 *     read_from_sock
 */
void TCP_Server::read_from_sock (int sock_fd)
{
    char buffer[Message_Queue::MESSAGE_SIZE] = {0};
    while(m_listening)
    {
        int valread = read( sock_fd, buffer, Message_Queue::MESSAGE_SIZE);
        if(0 >= valread)
        {
            Logger::debug("[TCP_Server::read_from_sock] read "+std::to_string(valread)+" from socket descriptor "+std::to_string(sock_fd));
            return;
        }
        else
        {
            // convert char* to string
            buffer[valread] = '\0';
            std::string str_out (&buffer[0]);
            // save message on receive queue
            m_msg_queue->push(str_out, sock_fd);
        }
    }
    Logger::debug("[TCP_Server::read_from_sock] exiting thread.");
}

/*--------------------------------------------------------------------------
 * Function:
 *     connect
 */
void TCP_Server::connect(unsigned short port)
{
    if(m_listening)
    {
        disconnect();
    }

    // create socket file descriptor
    // IPv4 and Byte-Stream socket.
    m_server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (0 > m_server_fd)
    {
        Logger::crit("[TCP_Server::connect] socket failed. ");
        throw "tcp server error";
    }
    // forcefully attaching to socket to the port
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/getsockopt.html
    int opt = 1;
    if(0 != setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        Logger::crit("[TCP_Server::connect] setsocketopt. ");
        throw "tcp server error";
    }
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(port);

    // Forefully attach the socket to the port
    if(0>bind(m_server_fd, (struct sockaddr *)&m_address, sizeof(m_address)))
    {
        Logger::crit("[TCP_Server::connect] bind failed. ");
        throw "tcp server error";
    }
    if(listen(m_server_fd, 3)<0)
    {
        Logger::crit("[TCP_Server::connect] listen. ");
        throw "tcp server error";
    }
  
    m_listening = true;
    
    Logger::debug("[TCP_Server::connect] listening. ");
    //int addrlen = sizeof(m_address);
    //int sock = accept(m_server_fd, (struct sockaddr *)&m_address, (socklen_t*)&addrlen);
    //m_socket.push_back(sock);
    //Logger::debug("[TCP_Server::connect] accepted connection. ");

    // spin off a thread for accepting a connection
    //std::thread t1(&TCP_Server::read_from_sock,this,sock);
    //t1.detach();

    // spin off a thread to listen to more connections
    std::thread t2(&TCP_Server::accept_conn,this);
    t2.detach();
}

/*--------------------------------------------------------------------------
 * Function:
 *     disconnect
 */
void TCP_Server::disconnect()
{
    if(!m_listening)
    {
        Logger::warn("[TCP_Server::disconnect] Exiting TCP disconnect without doing anything.");
        return;
    }
    
    m_listening = false;
    // disabled further send and receive operations
    // this will return a failure if the socket was not in a session
    for(int sock: m_socket)
    {
        shutdown(sock,SHUT_RDWR);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *     do_send
 */
void TCP_Server::do_send(Message_Queue::message_t snd)
{
    // a return value of -1 indicates a broken connection
    // with errno of ECONNRESET or 'connection timed out'
    ssize_t num_bits = send(snd.fd, snd.msg.c_str(), snd.msg.size(), 0);
    if(0 > num_bits)
    {
        Logger::warn("[TCP_Server::do_send] send error: "+std::to_string(errno));
    }
}

