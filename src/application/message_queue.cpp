/**-------------------------------------------------------------------------
 * @file message_queue.cpp
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
#include "application/message_queue.h"

/*--------------------------------------------------------------------------
 * Function:
 *     make 
 */
Message_Queue::sptr Message_Queue::make()
{
    return Message_Queue::sptr(new Message_Queue());
}

/*--------------------------------------------------------------------------
 * Function:
 *     Message_Queue
 */
Message_Queue::Message_Queue()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     ~Message_Queue
 */
Message_Queue::~Message_Queue()
{
}

/*--------------------------------------------------------------------------
 * Function:
 *     push
 */
bool Message_Queue::push(std::string msg, int fd )
{
    bool rval = false;
    message_t msg_struct;
    msg_struct.fd = fd;

    if( MESSAGE_SIZE < msg.length())
    {
        msg_struct.msg = msg.substr(0,MESSAGE_SIZE );
        rval = false;
    }
    else
    {
        msg_struct.msg = msg;
        rval = true;
    }

    auto lock = std::unique_lock<std::mutex>(m_mutex);
    m_queue.push(msg_struct);
    lock.unlock();

    return rval;
}
bool Message_Queue::push(Message_Queue::message_t msg)
{
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    m_queue.push(msg);
    lock.unlock();

    return true;
}

/*--------------------------------------------------------------------------
 * Function:
 *     pop_struct
 */
Message_Queue::message_t Message_Queue::pop_struct( void )
{
    message_t msg_struct;
    auto lock = std::unique_lock<std::mutex>(m_mutex);

    if(!m_queue.empty())
    {
        msg_struct = m_queue.front();
        m_queue.pop();
    }

    lock.unlock();

    return msg_struct;
}

/*--------------------------------------------------------------------------
 * Function:
 *     empty
 */
bool Message_Queue::empty()
{
    bool rval;
    auto lock = std::unique_lock<std::mutex>(m_mutex);

    rval = m_queue.empty();

    lock.unlock();

    return rval; 
}

/*--------------------------------------------------------------------------
 * Function:
 *     size
 */
int Message_Queue::size()
{
    int rval;
    auto lock = std::unique_lock<std::mutex>(m_mutex);

    rval = m_queue.size();

    lock.unlock();

    return rval; 
}


