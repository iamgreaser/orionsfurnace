#include "loops/game.h"

#include "core/core.h"
#include "core/player.h"
#include "gfx/gfx.h"
#include "loops/loops.h"

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
	int c_tile_flat = makecol(170, 170, 170);
	int c_tile_light = makecol(255, 255, 255);
	int c_tile_dark = makecol(85, 85, 85);
	for (int cy = 0; cy < CAM_H_CELLS; cy++) {
		int py = CAM_Y + (cy*CELL_H);

		for (int cx = 0; cx < CAM_W_CELLS; cx++) {
			int px = CAM_X + (cx*CELL_W);

			rectfill(backbuf, px, py, px+CELL_W-1, py+CELL_H-1, c_tile_flat);
			vline(backbuf,
				px+CELL_W*0/3, py+CELL_H*0/3, py+CELL_H*1/3,
				c_tile_light);
			hline(backbuf,
				px+CELL_W*0/3, py+CELL_H*0/3, px+CELL_W*1/3,
				c_tile_light);
			vline(backbuf,
				px+CELL_W*3/3-1, py+CELL_H*2/3-1, py+CELL_H*3/3-1,
				c_tile_dark);
			hline(backbuf,
				px+CELL_W*2/3-1, py+CELL_H*3/3-1, px+CELL_W*3/3-1,
				c_tile_dark);
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
