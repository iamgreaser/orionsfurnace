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

#ifndef NET_PACKET_CLIENT_HELLO_H
#define NET_PACKET_CLIENT_HELLO_H

#include "core/save.h"
#include "net/packet/packet.h"

#include <iostream>
#include <sstream>
#include <string>

namespace net
{
  class ClientHello final : public Saveable
  {
  private:
    std::string m_version;
    std::string m_nickname;
  public:
    ClientHello(std::string nickname = "");
    ClientHello(std::istream &ips);

    std::string get_version(void) {
      return m_version;
    }
    bool is_current_version(void);

    void load_this(std::istream &ips) override;
    void save_this(std::ostream &ops) const override;
  };

  typedef class ClassPacket<packets::HELLO, ClientHello> HelloPacket;
}

#endif /* if !defined(NET_PACKET_CLIENT_HELLO_H) */

