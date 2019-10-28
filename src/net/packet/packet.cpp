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

#include "net/packet/packet.h"

#include "core/version.h"

using net::ClientHello;

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

void ClientHello::save_this(std::ostream &ops) const
{
  save(ops, m_version);
  save(ops, m_nickname);
}
