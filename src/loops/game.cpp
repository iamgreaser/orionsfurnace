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
	clear_to_color(backbuf, makecol(85,85,255));

	// Draw things
	draw_playfield();
	draw_sidebar();
	draw_gui();
}

void loops::GameLoop::draw_playfield(void)
{
	// Set clipping rectangle
	set_clip_rect(backbuf,
		CAM_X,
		CAM_Y,
		CAM_W-1,
		CAM_H-1);
	set_clip_state(backbuf, 1);

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
	set_clip_state(backbuf, 0);
}

void loops::GameLoop::draw_sidebar(void)
{
	// Set clipping rectangle
	set_clip_rect(backbuf,
		SIDEBAR_X,
		SIDEBAR_Y,
		SIDEBAR_X+SIDEBAR_W-1,
		SIDEBAR_Y+SIDEBAR_H-1);
	set_clip_state(backbuf, 1);

	// Draw our sidebar
	int c_bg = makecol(0, 0, 0);
	int c_fg = makecol(170, 170, 255);
	clear_to_color(backbuf, c_bg);
	textprintf_ex(backbuf, font,
		SIDEBAR_X+1*8,
		SIDEBAR_Y+1*8,
		c_fg, c_bg,
		"Player pos: (%d, %d)",
		player.get_x(),
		player.get_y());

	// Clear clipping rectangle
	set_clip_state(backbuf, 0);
}

void loops::GameLoop::draw_gui(void)
{
	// TODO!
}
