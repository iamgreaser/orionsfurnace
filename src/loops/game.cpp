#include "loops/game.h"

#include "gfx/gfx.h"
#include "loops/loops.h"

#include <allegro.h>

namespace loops
{
}

loops::MainLoopState loops::game_loop(bool state_changed)
{
	(void)state_changed;

	if(key[KEY_ESC]) {
		return mainloop::EXIT;
	}

	clear_to_color(
		gfx::backbuf,
		makecol(0,0,170));

	return mainloop::GAME;
}
