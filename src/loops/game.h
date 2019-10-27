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

#ifndef LOOPS_GAME_H
#define LOOPS_GAME_H

#include "loops/loops.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "net/client.h"
#include "net/net.h"
#include "net/pipe.h"
#include "net/server.h"
#include "net/tcp.h"

#ifdef LOOPS_GAME_INTERNAL
union SDL_Event;
#endif

#include <fstream>
#include <memory>
#include <string>

#define USE_LOCAL_PIPES 1

namespace loops
{
  class GameLoop : public Loop
  {
  private:
    PlayerInput m_player_inputs;
#if USE_LOCAL_PIPES
    net::RawPipe m_local_pipe;
#else
#endif
    std::shared_ptr<net::Client> m_client;
    std::shared_ptr<net::Server> m_server;

    void tick_key_event(union SDL_Event &ev);
    void draw_playfield(void);
    void draw_sidebar(void);
    void draw_gui(void);
  public:
    GameLoop(void);
    virtual ~GameLoop(void);

    void start_server(int port);
    void start_client(std::string addr, int port);

    MainLoopState tick(void) override;
    void draw(void) override;
  };
}

#endif /* if !defined(_LOOPS_GAME_H) */
