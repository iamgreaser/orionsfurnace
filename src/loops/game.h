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

#ifdef _LOOPS_GAME_INTERNAL
#include <SDL.h>
#endif

#include <fstream>
#include <string>

#define USE_LOCAL_PIPES 0
#define USE_EXTRA_PLAYER 0

namespace loops
{
	class GameLoop : public Loop
	{
	private:
		PlayerInput m_player_inputs[2];
#if USE_LOCAL_PIPES
		net::RawPipe m_local_pipe;
#if USE_EXTRA_PLAYER
		net::RawPipe m_local_pipe_extra1;
#endif
#else
#endif
		net::Client *m_client = NULL;
		net::Server *m_server = NULL;

#if USE_EXTRA_PLAYER
		net::Client *m_client_extra1 = NULL;
#endif
	public:
		GameLoop(void);
		~GameLoop(void);

		void start_server(int port);
		void start_client(std::string addr, int port);

		MainLoopState tick(void) override;
		void draw(void) override;

	protected:
#ifdef _LOOPS_GAME_INTERNAL
		void tick_key_event(SDL_Event &ev);
		void draw_playfield(void);
		void draw_sidebar(void);
		void draw_gui(void);
#endif
	};
}

#endif /* if !defined(_LOOPS_GAME_H) */
