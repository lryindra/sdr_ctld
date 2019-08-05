/**-------------------------------------------------------------------------
 * @file message_server.cpp
 * @brief
 *
 * @copyright \{ © COPYRIGHT GNU General Public License ver. 3\}
 *------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <iostream>
#include "application/message_server.h"
#include "application/logger.h"
#include "application/command_msg.h"

/*--------------------------------------------------------------------------
 * Function:
 *     Message_Server
 */
Message_Server::Message_Server(Message_Queue::sptr cmd_queue, Message_Queue::sptr rsp_queue, int port_number)
    : m_tcp_port(port_number),
      m_partial_msg("")
{
    // print the UDP port number
    Logger::info("Listening on tcp port number: "+std::to_string(m_tcp_port));
    m_cmd_queue = cmd_queue;
    m_rsp_queue = rsp_queue;
    m_queue = Message_Queue::make();
    m_server = std::move(TCP_Server::uptr(new TCP_Server(m_queue)));
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~Message_Server
 */
Message_Server::~Message_Server()
{
    disconnect();
}

/*--------------------------------------------------------------------------
 * Function:
 *     listen
 */
void Message_Server::listen()
{
    Message_Queue::message_t msg_struct;
    std::string msg;
    while( !m_queue->empty() )
    {
        // remove the received message from the queue
        msg_struct = m_queue->pop_struct();
        msg = msg_struct.msg;
        m_partial_msg += msg;
        unsigned int size = m_partial_msg.size();
        if(Command_Msg::delim  == m_partial_msg[--size])
        {
            // this message was a complete command
            msg = m_partial_msg;
            m_partial_msg = "";
            //verify the message
            if(Command_Msg::check(msg))
            {
                msg_struct.msg = msg;
                m_cmd_queue->push(msg_struct);
            }
            else
            {
                Logger::debug("[Message_Server::listen] Bad packet "+msg);
                // return the message to the sender
                msg_struct.msg = "RPRT -11\n";
                m_server->do_send(msg_struct);
            }
        }
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *     send_response
 */
void Message_Server::send_response()
{
    while( !m_rsp_queue->empty() )
    {
        Message_Queue::message_t msg = m_rsp_queue->pop_struct();
        // return the message to the sender
        m_server->do_send(msg);
    }
}

/*--------------------------------------------------------------------------
 * Function:
 *     connect
 */
void Message_Server::connect()
{
    m_server->connect(m_tcp_port);
}

/*--------------------------------------------------------------------------
 * Function:
 *     disconnect
 */
void Message_Server::disconnect()
{
    m_server->disconnect();
}


