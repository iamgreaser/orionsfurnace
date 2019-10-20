#include "loops/game.h"

#include "core/core.h"
#include "core/player.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"
#include "loops/loops.h"

#include "gfx/internal.h"

#include <allegro.h>

namespace loops
{
	Player player(7, 7, direction::SOUTH);
}

loops::MainLoopState loops::GameLoop::tick(void)
{
	//
	// Update input
	//
	if (key[KEY_ESC]) {
		return mainloop::EXIT;
	}

	player.set_input_move(direction::NORTH, key[KEY_UP]);
	player.set_input_move(direction::SOUTH, key[KEY_DOWN]);
	player.set_input_move(direction::WEST,  key[KEY_LEFT]);
	player.set_input_move(direction::EAST,  key[KEY_RIGHT]);

	//
	// Update logic
	//
	player.tick();

	// Continue with the game
	return mainloop::GAME;
}

void loops::GameLoop::draw(void)
{
	// Clear screen
	gfx::clear(85,85,255);

	// Draw things
	draw_playfield();
	draw_sidebar();
	draw_gui();
}

void loops::GameLoop::draw_playfield(void)
{
	// Set clipping rectangle
	gfx::clip_rect(
		CAM_X, CAM_Y,
		CAM_W, CAM_H);

	// Draw tiles
	for (int cy = 0; cy < CAM_H_CELLS; cy++) {
		int py = CAM_Y + (cy*CELL_H);

		for (int cx = 0; cx < CAM_W_CELLS; cx++) {
			int px = CAM_X + (cx*CELL_W);

			gfx::tile_gfx_floor.draw(px, py);
		}
	}

	// Draw players
	player.draw();

	// Clear clipping rectangle
	gfx::clip_nothing();
}

void loops::GameLoop::draw_sidebar(void)
{
	// Set clipping rectangle
	gfx::clip_rect(
		SIDEBAR_X, SIDEBAR_Y,
		SIDEBAR_W, SIDEBAR_H);

	// Draw our sidebar
	int c_fg = makecol(170, 170, 255);
	int c_bg = makecol(  0,   0,   0);
	gfx::clear(0, 0, 0);
	textprintf_ex(backbuf, font,
		SIDEBAR_X+1*8,
		SIDEBAR_Y+1*8,
		c_fg, c_bg,
		"Player pos: (%d, %d)",
		player.get_x(),
		player.get_y());

	// Clear clipping rectangle
	gfx::clip_nothing();
}

void loops::GameLoop::draw_gui(void)
{
	// TODO!
}
