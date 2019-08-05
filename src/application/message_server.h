/**-------------------------------------------------------------------------
 * @file message_server.h
 * @brief
 *
 * @copyright \{ © COPYRIGHT GNU General Public License ver. 3\}
 * -------------------------------------------------------------------------
 */
#ifndef __MESSAGE_SERVER_H__
#define __MESSAGE_SERVER_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include "application/message_queue.h"
#include "application/tcp_server.h"

class Message_Server
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    /** @brief create and start the interface
     *
     * create a message interface on port_number
     *
     * @param cmd_queue - receive message queue
     * @param rsp_queue - outgoing message queue
     * @param port_number - the TCP Port number to listen to
     */
    Message_Server(Message_Queue::sptr cmd_queue, Message_Queue::sptr rsp_queue, int port_number);

    /** @brief disconnect interface
     *
     * disconnect and shutdown this message interface
     *
     */
    ~Message_Server();

    /** @brief Check packets for command messages.  
     *
     * Reply to packets with no command messages, and queue command messages.
     *
     * @return Void.
     */
    void listen();

    /** @brief send command response back to host
     *
     * @return Void.
     */
    void send_response();

    /** @brief send connect to TCP server
     *
     * @param Void.
     * @return Void.
     */
    void connect();

    /** @brief send disconnect to TCP server
     *
     * @param Void.
     * @return Void.
     */
    void disconnect();

private:
    int m_tcp_port;
    TCP_Server::uptr m_server;
    Message_Queue::sptr m_queue;
    Message_Queue::sptr m_cmd_queue;
    Message_Queue::sptr m_rsp_queue;
    std::string m_partial_msg;
};

#endif /*  __MESSAGE_SERVER_H__ */
