#include "loops/game.h"

#include "core/core.h"
#include "core/player.h"
#include "gfx/gfx.h"
#include "loops/loops.h"

#include <allegro.h>

namespace loops
{
	Player player(24*7, 24*7, direction::SOUTH);
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
	set_clip_rect(backbuf, 0, 0, 15*24-1, 15*24-1);
	set_clip_state(backbuf, 1);

	// Draw tiles
	int c_tile_flat = makecol(170, 170, 170);
	int c_tile_light = makecol(255, 255, 255);
	int c_tile_dark = makecol(85, 85, 85);
	for (int y = 0; y < 15; y++) {
		int py = y*24;

		for (int x = 0; x < 15; x++) {
			int px = x*24;

			rectfill(backbuf, px, py, px+24-1, py+24-1, c_tile_flat);
			vline(backbuf, px+ 0, py+ 0, py+ 8, c_tile_light);
			hline(backbuf, px+ 0, py+ 0, px+ 8, c_tile_light);
			vline(backbuf, px+23, py+15, py+23, c_tile_dark);
			hline(backbuf, px+15, py+23, px+23, c_tile_dark);
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
	set_clip_rect(backbuf, 15*24, 0, 640-1, 15*24-1);
	set_clip_state(backbuf, 1);

	// Draw our sidebar
	int c_bg = makecol(0, 0, 0);
	int c_fg = makecol(170, 170, 255);
	clear_to_color(backbuf, c_bg);
	textprintf_ex(backbuf, font, 15*24+1*8, 0+1*8,
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
