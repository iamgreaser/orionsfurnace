#define _LOOPS_GAME_INTERNAL 1
#include "loops/game.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "core/save.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"
#include "loops/loops.h"
#include "net/client.h"
#include "net/net.h"
#include "net/server.h"

#include <SDL.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

using loops::GameLoop;

GameLoop::GameLoop(void)
{
	m_client = new net::Client(m_stream_s2c, m_stream_c2s);
	m_server = new net::Server();
	m_server->add_client(m_stream_c2s, m_stream_s2c);

	std::stringstream ss;
	save(ss, m_server->game());
	ss.seekg(0);
	load(ss, m_client->game());
}

GameLoop::~GameLoop(void)
{
	if (m_demo_fp != NULL) {
		m_demo_fp->close();
		delete m_demo_fp;
		m_demo_fp = NULL;
	}

	if (m_client != NULL) {
		delete m_client;
		m_client = NULL;
	}

	if (m_server != NULL) {
		delete m_server;
		m_server = NULL;
	}
}

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
	// TEST: Start recording demo if we haven't yet
	//
	if (m_demo_fp == NULL) {
		m_demo_fp = new std::ofstream("test.demo");
		net::GameSnapshotPacket game_snapshot_packet(m_server->game());
		save(*m_demo_fp, game_snapshot_packet);
	}

	//
	// Update logic
	// TODO: Separate input handling between client and server
	//
	assert(m_server != NULL);
	GameFrame game_frame(m_server->game().get_player_count());
	game_frame.player_set_all_inputs(0, m_player_inputs[0]);
	game_frame.player_set_all_inputs(1, m_player_inputs[1]);
	m_server->game_tick(game_frame);
	m_server->update();

	// Update client logic
	// TODO: Shove this into the client
	if (m_client != NULL) {
		m_client->update();
		m_client->game().tick(game_frame);
	}

	//
	// TEST: Add input to demo
	//
	{
		net::GameFramePacket game_frame_packet(game_frame);
		save(*m_demo_fp, game_frame_packet);
	}

	// Continue with the game
	return mainloop::GAME;
}

void GameLoop::tick_key_event(SDL_Event &ev)
{
	switch(ev.key.keysym.sym)
	{
		case SDLK_w:
			m_player_inputs[0].set_input_move(
				direction::NORTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_s:
			m_player_inputs[0].set_input_move(
				direction::SOUTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_a:
			m_player_inputs[0].set_input_move(
				direction::WEST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_d:
			m_player_inputs[0].set_input_move(
				direction::EAST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_UP:
			m_player_inputs[1].set_input_move(
				direction::NORTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_DOWN:
			m_player_inputs[1].set_input_move(
				direction::SOUTH,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_LEFT:
			m_player_inputs[1].set_input_move(
				direction::WEST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_RIGHT:
			m_player_inputs[1].set_input_move(
				direction::EAST,
				ev.type == SDL_KEYDOWN);
			break;

		case SDLK_F2:
			if (ev.type == SDL_KEYDOWN) {
				std::ofstream fp("quick.save");
				save(fp, m_server->game());
				fp.close();
			}
			break;

		case SDLK_F3:
			if (ev.type == SDL_KEYDOWN) {
				std::ifstream fp("quick.save");
				load(fp, m_server->game());
				fp.close();

				// Also load in demo
				if (m_demo_fp == NULL) {
					m_demo_fp = new std::ofstream("test.demo");
				}
				net::GameSnapshotPacket game_snapshot_packet(m_server->game());
				save(*m_demo_fp, game_snapshot_packet);

				// FIXME: Load this from the network
				if (m_client != NULL) {
					std::ifstream fp2("quick.save");
					load(fp2, m_client->game());
					fp2.close();
				}
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
	if (m_client != NULL) {
		m_client->game().draw();
	}

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

	if (m_client != NULL) {
		for (int pidx = 0; pidx < m_client->game().get_player_count(); pidx++)
		{
			Player *player = m_client->game().get_player_ptr(pidx);

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
	}

	// Clear clipping rectangle
	gfx::clip_nothing();
}

void GameLoop::draw_gui(void)
{
	// TODO!
}
