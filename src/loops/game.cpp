#include "loops/game.h"

#include "core/core.h"
#include "core/player.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"
#include "loops/loops.h"

#include <SDL.h>

#include <sstream>

namespace loops
{
	Player player(7, 7, direction::SOUTH);
}

loops::MainLoopState loops::GameLoop::tick(void)
{
	//
	// Update input
	//
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
			case SDL_QUIT:
				return mainloop::EXIT;

			case SDL_KEYUP:
			case SDL_KEYDOWN:
			switch(ev.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					if (ev.type == SDL_KEYUP) {
						return mainloop::EXIT;
					}
					break;

				case SDLK_UP:
					player.set_input_move(
						direction::NORTH,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_DOWN:
					player.set_input_move(
						direction::SOUTH,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_LEFT:
					player.set_input_move(
						direction::WEST,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_RIGHT:
					player.set_input_move(
						direction::EAST,
						ev.type == SDL_KEYDOWN);
					break;

			} break;
		}
	}

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
	gfx::clear(0, 0, 0);
	std::stringstream ss;
	ss << "Player pos: (";
	ss << player.get_x() << ", ";
	ss << player.get_y() << ")";
	gfx::draw_text(
		SIDEBAR_X+1*8,
		SIDEBAR_Y+1*8,
		170, 170, 255,
		ss.str());

	// Clear clipping rectangle
	gfx::clip_nothing();
}

void loops::GameLoop::draw_gui(void)
{
	// TODO!
}
