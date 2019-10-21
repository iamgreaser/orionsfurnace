#ifndef _LOOPS_GAME_H
#define _LOOPS_GAME_H

#include "loops/loops.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"

#ifdef _LOOPS_GAME_INTERNAL
#include <SDL.h>
#endif

namespace loops
{
	class GameLoop : public Loop
	{
	private:
		Game m_game;
		PlayerInput m_player_inputs[2];
	public:
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
