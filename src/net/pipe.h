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

#ifndef NET_PIPE_H
#define NET_PIPE_H

#include "net/net.h"

#include <iostream>
#include <memory>
#include <sstream>

namespace net
{
  class RawPipeEnd final : public PipeEnd
  {
  protected:
    std::shared_ptr<std::stringstream> m_send_ss;
    std::shared_ptr<std::stringstream> m_recv_ss;
  public:
    RawPipeEnd(std::shared_ptr<std::stringstream> ips, std::shared_ptr<std::stringstream> ops)
      : m_send_ss(ips)
      , m_recv_ss(ops)
    {
      m_send_ss = ips;
      m_recv_ss = ops;
    }
    std::ostream &send_stream(void) override {
      return static_cast<std::ostream&>(*m_send_ss);
    }
    std::istream &recv_stream(void) override {
      return static_cast<std::istream&>(*m_recv_ss);
    }
  };

  class RawPipe
  {
  protected:
    std::shared_ptr<std::stringstream> m_a_to_b;
    std::shared_ptr<std::stringstream> m_b_to_a;
    std::shared_ptr<RawPipeEnd> m_end_a;
    std::shared_ptr<RawPipeEnd> m_end_b;
  public:
    RawPipe(void)
      : m_a_to_b(new std::stringstream())
      , m_b_to_a(new std::stringstream())
      , m_end_a(new RawPipeEnd(m_b_to_a, m_a_to_b))
      , m_end_b(new RawPipeEnd(m_a_to_b, m_b_to_a))
    {
    }
    std::shared_ptr<PipeEnd> end_a(void) { return m_end_a; }
    std::shared_ptr<PipeEnd> end_b(void) { return m_end_b; }
  };
}

#endif /* if !defined(NET_PIPE_H) */
