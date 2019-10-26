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

#ifndef _LOOPS_GAME_H
#define _LOOPS_GAME_H

#include "loops/loops.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "net/client.h"
#include "net/net.h"
#include "net/pipe.h"
#include "net/server.h"

#ifdef _LOOPS_GAME_INTERNAL
#include <SDL.h>
#endif

#include <fstream>

namespace loops
{
	class GameLoop : public Loop
	{
	private:
		PlayerInput m_player_inputs[2];
		net::RawPipe m_local_pipe;
		net::Client *m_client = NULL;
		net::Server *m_server = NULL;

		net::Client *m_client_extra1 = NULL;
		net::RawPipe m_local_pipe_extra1;
	public:
		GameLoop(void);
		~GameLoop(void);
		MainLoopState tick(void);
		void draw(void);

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
