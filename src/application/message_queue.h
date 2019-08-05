/**-------------------------------------------------------------------------
 * @file message_queue.h
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
#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

/*--------------------------------------------------------------------------
 * Include Files
 * -----------------------------------------------------------------------*/
#include <queue>
#include <mutex>
#include <string>
#include <memory>

class Message_Queue
{
public:
/*--------------------------------------------------------------------------
 * Type Definitions
 * -----------------------------------------------------------------------*/
    static const int MESSAGE_SIZE=1400;/**< Maximum size of message in queue */

    /** shared pointer to message queue */
    typedef std::shared_ptr<Message_Queue> sptr;

    struct {
        std::string msg;
        int fd;
    } typedef message_t;

/*--------------------------------------------------------------------------
 * Function Definitions
 * -----------------------------------------------------------------------*/
    /** @brief make a shared pointer
     *
     * @return sptr
     */
    static sptr make();

protected:
    /** @brief Constructor
     *
     * build queue instance
     *
     */
    Message_Queue();

public:
    /** @brief Deconstructor
     *
     * delete queue instance
     *
     */
    ~Message_Queue();

    /** @brief pushes the message on to the queue.  return false if the 
     * message was trunkated because it was too long.
     *
     * @param msg
     * @return bool
     */
    bool push(std::string msg, int fd);
    bool push(message_t msg);

    /** @brief returns the next struct from the queue
     *
     * @return message_t
     */
    message_t pop_struct( void );

    /** @brief is queue empty
     *
     * true if the queue is empty
     *
     * @return bool
     */
    bool empty();

    /** @brief return the size of the queue
     *
     * @return int
     */
    int size();

private:
    std::queue <message_t> m_queue;
    std::mutex m_mutex;
};

#endif /* __MESSAGE_QUEUE_H__ */
