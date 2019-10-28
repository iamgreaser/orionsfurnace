/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef NET_NET_H
#define NET_NET_H

#include "core/core.h"
#include "core/save.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>


namespace net
{
  class Client;
  class ClientHello;
  class Packet;
  class PipeEnd;
  class Server;

  class PipeEnd
  {
  protected:
    bool m_disconnected = false;
    std::string m_disconnect_message = "";
  public:
    virtual ~PipeEnd(void) {}

    bool is_disconnected(void) {
      return m_disconnected;
    }
    std::string get_disconnect_message(void) {
      return m_disconnect_message;
    }

    virtual std::ostream &send_stream(void) = 0;
    virtual std::istream &recv_stream(void) = 0;
    virtual void pump_recv(void) {}
    virtual void pump_send(void) {}
  };


  class Node
  {
  protected:
    std::shared_ptr<net::PipeEnd> m_pipe_end;
    std::string m_input_buf;
  public:
    Node(std::shared_ptr<net::PipeEnd> pipe_end);
    void send_packet(net::Packet &packet);
    virtual ~Node(void);
    virtual void update(void) = 0;
    void update_packets(void);
    virtual void handle_input_packet(int packet_id, std::istream &packet_ss) = 0;
  };

}

#endif /* if !defined(NET_NET_H) */
