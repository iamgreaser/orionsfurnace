#define _LOOPS_GAME_INTERNAL 1
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
#include <iostream>
#include <sstream>

namespace loops
{
	Game game;
	PlayerInput player_inputs[2];
}

using loops::GameLoop;

loops::MainLoopState GameLoop::tick(void)
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
				if (ev.key.keysym.sym == SDLK_ESCAPE && ev.type == SDL_KEYUP) {
					return mainloop::EXIT;
				} else {
					this->tick_key_event(ev);
				}
				break;
		}
	}

	//
	// Update logic
	//
	GameFrame game_frame(game.get_player_count());
	game_frame.player_set_all_inputs(0, player_inputs[0]);
	game_frame.player_set_all_inputs(1, player_inputs[1]);
	game.tick(game_frame);

	// TEST: Save then load the game
	std::stringstream game_ss;
	save(game_ss, game);
	{
		std::ofstream fp("test.save");
		game_ss.seekg(0);
		// FIXME: get this working
		//fp << (std::istream)game_ss;
		fp << game_ss.str();
		std::cout << "Save: " << game_ss.str().size() << " bytes" << std::endl;
		fp.close();
	}
	game_ss.seekg(0);
	load(game_ss, game);

	// Continue with the game
	return mainloop::GAME;
}

void GameLoop::tick_key_event(SDL_Event &ev)
{
	switch(ev.key.keysym.sym)
	{
		case SDLK_w:
			player_inputs[0].set_input_move(
				direction::NORTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_s:
			player_inputs[0].set_input_move(
				direction::SOUTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_a:
			player_inputs[0].set_input_move(
				direction::WEST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_d:
			player_inputs[0].set_input_move(
				direction::EAST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_UP:
			player_inputs[1].set_input_move(
				direction::NORTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_DOWN:
			player_inputs[1].set_input_move(
				direction::SOUTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_LEFT:
			player_inputs[1].set_input_move(
				direction::WEST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_RIGHT:
			player_inputs[1].set_input_move(
				direction::EAST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_F2:
			if(ev.type == SDL_KEYDOWN) {
				std::ofstream fp("quick.save");
				save(fp, game);
				fp.close();
			}
			break;

		case SDLK_F3:
			if(ev.type == SDL_KEYDOWN) {
				std::ifstream fp("quick.save");
				load(fp, game);
				fp.close();
			}
			break;

	}
}

void GameLoop::draw(void)
{
	// Clear screen
	gfx::clear(85,85,255);

	// Draw things
	draw_playfield();
	draw_sidebar();
	draw_gui();
}

void GameLoop::draw_playfield(void)
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

void GameLoop::draw_sidebar(void)
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

void GameLoop::draw_gui(void)
{
	// TODO!
}
