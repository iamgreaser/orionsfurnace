#include "loops/game.h"

#include "core/core.h"
#include "gfx/gfx.h"
#include "loops/loops.h"

#include <allegro.h>

namespace loops
{
	int player_x = 320-12;
	int player_y = 180-12;
	Direction player_direction = direction::SOUTH;
}

loops::MainLoopState loops::game_loop(bool state_changed)
{
	(void)state_changed;

	//
	// Update input
	//
	if (key[KEY_ESC]) {
		return mainloop::EXIT;
	}

	int player_dx = 0;
	int player_dy = 0;
	if (key[KEY_UP]) { player_dy -= 1; }
	if (key[KEY_DOWN]) { player_dy += 1; }
	if (key[KEY_LEFT]) { player_dx -= 1; }
	if (key[KEY_RIGHT]) { player_dx += 1; }

	//
	// Update logic
	//
	if (player_dy < 0) { player_direction = direction::NORTH; }
	if (player_dy > 0) { player_direction = direction::SOUTH; }
	if (player_dx < 0) { player_direction = direction::WEST; }
	if (player_dx > 0) { player_direction = direction::EAST; }

	player_x += player_dx;
	player_y += player_dy;

	//
	// Draw
	//
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

	draw_sprite(backbuf,
		gfx::player_gfx[player_direction],
		player_x, player_y);

	return mainloop::GAME;
}
