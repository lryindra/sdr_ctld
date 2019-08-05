/**-------------------------------------------------------------------------
 * @file tcp_server.h 
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
* --------------------------------------------------------------------------
*/
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "application/message_queue.h"
#include <netinet/in.h>
#include <thread>
#include <string>
#include <vector>

class TCP_Server
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    /** unique pointer to message queue */
    typedef std::unique_ptr<TCP_Server> uptr;

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    /** @brief Constructor for the UDP Socket
     *
     * @param msg_queue - shared pointer for the receive queue
     */
    TCP_Server(Message_Queue::sptr msg_queue);

    /** @brief Deconstructor for the UDP Socket
     *
     */
    ~TCP_Server();

public:
    /** @brief connect
     *
     * throws "tcp server error" if the server failed
     *
     * @param port - the UDP port number for listen
     * @return Void.
     */
    void connect(unsigned short port);

    /** @brief disconnect
     *
     * @return Void.
     */
    void disconnect();

    /** @brief Send
     *
     * Send msg as a return packet
     *
     * @param snd - the message to send
     * @return Void.
     */
    void do_send(Message_Queue::message_t snd);

private:
    int m_server_fd;
    std::vector<int> m_socket;
    struct sockaddr_in m_address;
    bool m_listening;
    Message_Queue::sptr m_msg_queue;
    enum { max_length = Message_Queue::MESSAGE_SIZE };

    /** @brief accept TCP connection
     *
     * accept connection to TCP server
     *
     * @param Void.
     * @return Void.
     */
    void accept_conn( void);

    /** @brief read from tcp socket
     *
     * function passed to std::thread; reads from socket file descriptor
     *
     * @param sock_fd - socket file descriptor
     * @return Void.
     */
    void read_from_sock (int sock_fd);
};

#endif /* __TCP_SERVER_H_ */ 
