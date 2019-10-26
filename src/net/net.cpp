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

#include "net/net.h"

#include "core/version.h"

using net::Node;
using net::ClientHello;

Node::Node(net::PipeEnd *pipe_end)
  : m_pipe_end(pipe_end)
{
}

Node::~Node(void)
{
  if (m_pipe_end != nullptr) {
    delete m_pipe_end;
    m_pipe_end = nullptr;
  }
}

void Node::send_packet(net::Packet &packet)
{
  save(m_pipe_end->send_stream(), packet);
}

void Node::update_packets(void)
{
  // Fetch some input
  {
    char buf[8192];
    std::streamsize sz = m_pipe_end->recv_stream().readsome(buf, 8192);
    m_input_buf += std::string(buf, sz);
  }

  // Extract packets
  for (;;) {
    size_t buffer_size = m_input_buf.size();

    // Do we have enough stuff to read to make any sense of this?
    if (buffer_size < 5) {
      // No - wait.
      break;
    }

    // Extract the bare minimum info needed
    uint32_t sub_size = 0;
    sub_size += static_cast<uint32_t>((m_input_buf[0]));
    sub_size += static_cast<uint32_t>((m_input_buf[1])<<8);
    sub_size += static_cast<uint32_t>((m_input_buf[2])<<16);
    sub_size += static_cast<uint32_t>((m_input_buf[3])<<24);
    assert(sub_size >= 1);
    uint8_t packet_id = static_cast<uint8_t>(m_input_buf[4]);
    size_t full_packet_size = 4+1+sub_size;

    // Will it fit?
    if (buffer_size < full_packet_size) {
      // No - leave it in the buffer.
      break;
    }

    // Get packet data.
    std::string packet_s(m_input_buf, 4+1, full_packet_size-(4+1));
    m_input_buf.erase(m_input_buf.begin(), m_input_buf.begin()+full_packet_size);
    assert(m_input_buf.size() != buffer_size);
    std::stringstream packet_ss(packet_s);

    // Handle packet.
    this->handle_input_packet(packet_id, packet_ss);
  }
}


ClientHello::ClientHello(std::string nickname)
  : m_version(get_engine_version())
  , m_nickname(nickname)
{
}

ClientHello::ClientHello(std::istream &ips)
{
  load(ips, *this);
}

bool ClientHello::is_current_version(void)
{
  return (m_version == get_engine_version());
}

void ClientHello::load_this(std::istream &ips)
{
  load(ips, m_version);
  load(ips, m_nickname);
}

void ClientHello::save_this(std::ostream &ops)
{
  save(ops, m_version);
  save(ops, m_nickname);
}
