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

#ifndef NET_SERVER_SERVER_CLIENT_H
#define NET_SERVER_SERVER_CLIENT_H

#include "net/net.h"
#include "core/core.h"
#include "core/game.h"
#include "core/player.h"

#include <memory>

namespace net
{
  namespace server_client_status
  {
    enum ServerClientStatus
    {
      // TODO: Use these!
      AWAITING_HELLO = 0,
      SENDING_YOUR_PLAYER,
      SENDING_GAME,
      PLAYING_GAME,
      DISCONNECTED,
    };
  }
  using server_client_status::ServerClientStatus;

  class ServerClient final : public Node
  {
  private:
    ServerClientStatus m_status;
    Server *m_server = nullptr;
    GameFrame m_game_frame;
    int m_player_idx;
    PlayerInput m_player_input;
    bool m_disconnected = false;
  public:
    ServerClient(Server *server, int player_idx, std::shared_ptr<net::PipeEnd> pipe_end);
    ~ServerClient(void);
    PlayerInput get_player_input(void) {
      return m_player_input;
    }
    int get_player_idx(void) {
      return m_player_idx;
    }
    bool is_disconnected(void) {
      return m_disconnected;
    }
    void update(void) override;
    void handle_input_packet(int packet_id, std::istream &packet_ss) override;
  };
}

#endif /* if !defined(NET_SERVER_SERVER_CLIENT_H) */
