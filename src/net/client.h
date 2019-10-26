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

#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include "core/core.h"
#include "core/game.h"
#include "core/save.h"

#include <iostream>
#include <sstream>
#include <string>

namespace net
{
  namespace client_status
  {
    enum ClientStatus
    {
      SENDING_HELLO = 0,
      LOADING_GAME,
      PLAYING_GAME,
      DISCONNECTED,
    };
  }
  using client_status::ClientStatus;

  class Client : public Node
  {
  private:
    ClientStatus m_status = client_status::SENDING_HELLO;
    PlayerInput m_player_input;
    int m_player_idx = -1;
    Game *m_game = nullptr;
    std::string m_disconnect_message = "";

    // Throttle input sending to once a frame
    bool m_ready_to_send_input = false;
  public:
    Client(net::PipeEnd *pipe_end);
    ~Client(void);

    Game *game(void);
    int get_player_idx(void);
    PlayerInput get_player_input(void);
    void set_all_inputs(PlayerInput player_input);
    std::string get_central_message(void);

    void load_game(std::istream &ips);

    void update(void) override;
    void tick_input_send(void);
    void handle_input_packet(int packet_id, std::istream &packet_ss) override;
  };
}

#endif /* if !defined(NET_CLIENT_H) */
