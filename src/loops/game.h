#ifndef _LOOPS_GAME_H
#define _LOOPS_GAME_H

#include "loops/loops.h"

#ifdef _LOOPS_GAME_INTERNAL
#include <SDL.h>
#endif

namespace loops
{
	class GameLoop : public Loop
	{
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
