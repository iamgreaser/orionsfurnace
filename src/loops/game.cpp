#include "loops/game.h"

#include "core/core.h"
#include "core/player.h"
#include "gfx/gfx.h"
#include "loops/loops.h"

#include <allegro.h>

namespace loops
{
	Player player(320-12, 180-12, direction::SOUTH);
}

loops::MainLoopState loops::GameLoop::tick(void)
{
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
	player.move(player_dx, player_dy);

	// Continue with the game
	return mainloop::GAME;
}

void loops::GameLoop::draw(void)
{
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

	player.draw();
}
