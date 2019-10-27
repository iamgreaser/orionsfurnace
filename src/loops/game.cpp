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
#include "net/client.h"
#include "net/net.h"
#include "net/server.h"

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
  m_client.reset(new net::Client(m_local_pipe.end_a()));
  if (m_server != nullptr) {
    // Direct connection
    m_server.get()->add_client(m_local_pipe.end_b());
  }
#else
  if (addr == "") {
    addr = "localhost";
  }
  std::shared_ptr<net::TCPPipeEnd> client_socket(
    new net::TCPPipeEnd(addr, port));
  m_client.reset(new net::Client(client_socket));
#endif
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

  // Draw tiles
  for (int cy = 0; cy < CAM_H_CELLS; cy++) {
    int py = CAM_Y + (cy*CELL_H);

    for (int cx = 0; cx < CAM_W_CELLS; cx++) {
      int px = CAM_X + (cx*CELL_W);

      gfx::tile_gfx_floor.draw(px, py);
    }
  }

  // Draw game
  if (m_client != nullptr) {
    Game *game = m_client->game();
    if (game != nullptr) {
      game->draw();
    }
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

  if (m_client != nullptr) {
    Game *game = m_client->game();
    if (game != nullptr) {
      for (int pidx = 0; pidx < game->get_player_count(); pidx++) {
        Player *player = game->get_player_ptr(pidx);

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
