#include "loops/game.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "core/save.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"
#include "loops/loops.h"

#include <SDL.h>

#include <fstream>
#include <sstream>

namespace loops
{
	Game game;
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

				case SDLK_w:
					game.set_player_input_move(
						0,
						direction::NORTH,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_s:
					game.set_player_input_move(
						0,
						direction::SOUTH,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_a:
					game.set_player_input_move(
						0,
						direction::WEST,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_d:
					game.set_player_input_move(
						0,
						direction::EAST,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_UP:
					game.set_player_input_move(
						1,
						direction::NORTH,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_DOWN:
					game.set_player_input_move(
						1,
						direction::SOUTH,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_LEFT:
					game.set_player_input_move(
						1,
						direction::WEST,
						ev.type == SDL_KEYDOWN);
					break;

				case SDLK_RIGHT:
					game.set_player_input_move(
						1,
						direction::EAST,
						ev.type == SDL_KEYDOWN);
					break;

			} break;
		}
	}

	//
	// Update logic
	//
	game.tick();

	// TEST: Save then load the game
	std::stringstream game_ss;
	save(game_ss, game);
	{
		std::ofstream fp("test.save");
		game_ss.seekg(0);
		// FIXME: get this working
		//fp << (std::istream)game_ss;
		fp << game_ss.str();
		fp.close();
	}
	game_ss.seekg(0);
	load(game_ss, game);

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

	// Draw game
	game.draw();

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

	for (int pidx = 0; pidx < game.get_player_count(); pidx++)
	{
		Player *player = game.get_player_ptr(pidx);

		std::stringstream ss;
		ss << "Player pos: (";
		ss << player->get_x() << ", ";
		ss << player->get_y() << ")";
		gfx::draw_text(
			SIDEBAR_X+1*8,
			SIDEBAR_Y+(1+pidx*1)*12,
			170, 170, 255,
			ss.str());
	}

	// Clear clipping rectangle
	gfx::clip_nothing();
}

void loops::GameLoop::draw_gui(void)
{
	// TODO!
}
