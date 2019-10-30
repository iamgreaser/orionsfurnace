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

#ifndef NET_SERVER_SERVER_H
#define NET_SERVER_SERVER_H

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "net/net.h"
#include "net/tcp.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

namespace net
{
  class ServerClient;

  class Server final
  {
  private:
    std::shared_ptr<std::ofstream> m_demo_fp;
    std::vector<std::shared_ptr<ServerClient>> m_clients;
    net::TCPServer m_tcp_server;
    std::shared_ptr<Game> m_game = std::make_shared<Game>();
    int m_next_player_idx = 0;
  public:
    Server(int port);
    ~Server(void);

    std::shared_ptr<Game> game(void) {
      return m_game;
    }

    void add_client(std::shared_ptr<net::PipeEnd> pipe_end);
    int register_new_player(void);

    void broadcast_packet(net::Packet &packet);
    void broadcast_packet_ignoring_client(net::Packet &packet, ServerClient *ignore_sc);

    void quicksave(void);
    void quickload(void);

    void game_tick(GameFrame game_frame);
    void update(void);
  };
}

#endif /* if !defined(NET_SERVER_SERVER_H) */
