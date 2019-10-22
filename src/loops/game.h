#ifndef _LOOPS_GAME_H
#define _LOOPS_GAME_H

#include "loops/loops.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "net/client.h"
#include "net/net.h"
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
		std::ofstream *m_demo_fp = NULL;
		std::stringstream m_stream_c2s;
		std::stringstream m_stream_s2c;
		net::Client *m_client = NULL;
		net::Server *m_server = NULL;
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
