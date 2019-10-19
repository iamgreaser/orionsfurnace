#include "loops/game.h"

#include "core/core.h"
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

	clear_to_color(backbuf, makecol(85,85,255));
	textprintf_ex(backbuf, font, 20, 10+8*0,
		makecol(255,255,0), -1,
		"Testing mobiletrash resolution");
	textprintf_ex(backbuf, font, 0, 10+8*2,
		makecol(255,255,0), -1,
		"-------------------------------------------------------------------------------|");
	textprintf_ex(backbuf, font, 20, 10+8*3,
		makecol(255,255,0), -1,
		"80 columns barely fit: TICK!");

	draw_sprite(backbuf, gfx::player_gfx[direction::SOUTH], 320-12, 180-12);

	return mainloop::GAME;
}
