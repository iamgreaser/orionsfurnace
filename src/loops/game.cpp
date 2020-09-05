/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#define LOOPS_GAME_INTERNAL 1
#include "loops/game.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "core/save.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"
#include "loops/loops.h"
#include "net/client/client.h"
#include "net/net.h"
#include "net/server/server.h"

#include <SDL.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using loops::GameLoop;

GameLoop::GameLoop(void)
{
}

GameLoop::~GameLoop(void)
{
}

void GameLoop::start_server(int port)
{
  assert(m_server == nullptr);
  m_server.reset(new net::Server(port));
}

void GameLoop::start_client(std::string addr, int port)
{
  (void)addr;
  (void)port;

  assert(m_client == nullptr);
#if USE_LOCAL_PIPES
  if (addr == "") {
    m_client.reset(new net::Client(m_local_pipe.end_a()));
    if (m_server != nullptr) {
      // Direct connection
      m_server.get()->add_client(m_local_pipe.end_b());
    }
    return;
  }
#else
  if (addr == "") {
    addr = "localhost";
  }
#endif

  std::shared_ptr<net::TCPPipeEnd> client_socket(
    new net::TCPPipeEnd(addr, port));
  m_client.reset(new net::Client(client_socket));
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
  // Send inputs
  //
  if (m_client != nullptr) {
    m_client.get()->set_all_inputs(m_player_inputs);
  }

  //
  // Update client logic
  //
  if (m_client != nullptr) {
    m_client.get()->update();
  }

  //
  // Update server logic
  //
  if (m_server != nullptr) {
    m_server.get()->update();
  }

  //
  // Update client logic
  //
  if (m_client != nullptr) {
    m_client.get()->update();
  }

  // Continue with the game
  return mainloop::GAME;
}

void GameLoop::tick_key_event(SDL_Event &ev)
{
  switch(ev.key.keysym.sym)
  {
    case SDLK_w:
      m_player_inputs.set_input_move(
        direction::NORTH,
        ev.type == SDL_KEYDOWN);
      break;

    case SDLK_s:
      m_player_inputs.set_input_move(
        direction::SOUTH,
        ev.type == SDL_KEYDOWN);
      break;

    case SDLK_a:
      m_player_inputs.set_input_move(
        direction::WEST,
        ev.type == SDL_KEYDOWN);
      break;

    case SDLK_d:
      m_player_inputs.set_input_move(
        direction::EAST,
        ev.type == SDL_KEYDOWN);
      break;

    case SDLK_F2:
      if (ev.type == SDL_KEYDOWN) {
        if (m_server != nullptr) {
          m_server->quicksave();
        }
      }
      break;

    case SDLK_F3:
      if (ev.type == SDL_KEYDOWN) {
        if (m_server != nullptr) {
          m_server->quickload();
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

  gfx::clear(40, 0, 20);

  // Draw game
  if (m_client != nullptr && m_client.get()->is_playing()) {
    std::shared_ptr<Game> game = m_client->game();
    Player *player = m_client->get_player_ptr();

    // Get camera centre
    int rel_px = 0;
    int rel_py = 0;
    if (player != nullptr) {
      Entity *e = player->get_entity();
      if (e != nullptr) {
        e->calc_interp_pos(&rel_px, &rel_py);
        // Actually make it the centre, rather than centering on the top-left of the player
        rel_px += CELL_W / 2;
        rel_py += CELL_H / 2;
        // Move from camera centre to top left
        rel_px -= (CAM_W_CELLS * CELL_W) / 2;
        rel_py -= (CAM_H_CELLS * CELL_H) / 2;
      }
    }

    gfx::set_camera(rel_px, rel_py);

    if (game != nullptr) {
      game.get()->draw();
    }
  }

  // Clear clipping rectangle
  gfx::clip_nothing();

  // Reset camera
  gfx::set_camera(0, 0);
}

void GameLoop::draw_sidebar(void)
{
  // Set clipping rectangle
  gfx::clip_rect(
    SIDEBAR_X, SIDEBAR_Y,
    SIDEBAR_W, SIDEBAR_H);

  // Draw our sidebar
  gfx::clear(0, 0, 0);

  if (m_client != nullptr) {
    std::shared_ptr<Game> game = m_client->game();
    if (game != nullptr) {
      int row_idx = 0;
      for (auto player_it = game.get()->player_begin(); player_it != game.get()->player_end(); player_it++) {
        int pidx = (*player_it).first;
        Player *player = game.get()->get_player_ptr(pidx);

        std::stringstream ss;
        ss << "Player " << pidx << " pos: (";
        ss << player->get_x() << ", ";
        ss << player->get_y() << ")";
        gfx::draw_text(
          SIDEBAR_X+1*8,
          SIDEBAR_Y+(1+row_idx*1)*12,
          170, 170, 255,
          ss.str());
        row_idx += 1;
      }
    }
  }

  // Clear clipping rectangle
  gfx::clip_nothing();
}

void GameLoop::draw_gui(void)
{
  // TODO: more stuff

  std::string central_message = "NO CLIENT";
  if (m_client != nullptr) {
    central_message = m_client->get_central_message();
  }

  if (central_message != "") {
    // Get dimensions
    int pw = 0;
    int ph = 0;
    gfx::fetch_text_dims(central_message, &pw, &ph);

    int px = (gfx::BASE_SCREEN_WIDTH-pw+1)/2;
    int py = (gfx::BASE_SCREEN_HEIGHT-ph+1)/2;

    // Draw background box
    constexpr int padw1 = 20;
    constexpr int padh1 = 10;
    constexpr int padw2 = padw1+5;
    constexpr int padh2 = padh1+5;
    gfx::draw_rect(
      px-padw2, py-padh2,
      pw+padw2*2, ph+padh2*2,
      170, 170, 255);
    gfx::draw_rect(
      px-padw1, py-padh1,
      pw+padw1*2, ph+padh1*2,
      0, 0, 0);

    // Draw message
    gfx::draw_text(
      px, py,
      170, 170, 255,
      central_message);
  }
}
