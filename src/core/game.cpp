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

#include "core/game.h"

#include "core/core.h"
#include "core/helpers.h"
#include "core/save.h"

#include <cstdint>
#include <iostream>
using std::istream;
using std::ostream;
#include <vector>
using std::vector;

namespace game
{
}

Game::Game(void)
{
}

Game::Game(std::istream &ips)
{
  load(ips, *this);
}

Game::~Game(void)
{
}

void Game::add_player(Player player)
{
  int player_idx = this->get_player_count();
  m_players.push_back(player);
  m_players[player_idx].set_game(this);
}

void Game::player_set_all_inputs(int player_idx, PlayerInput player_input)
{
  assert(player_idx >= 0 && player_idx < this->get_player_count());
  m_players[player_idx].set_all_inputs(player_input);
}

bool Game::can_step_into(int cx, int cy, bool players_are_blocking)
{
  // Is this in range of the map?
  if (cx < 0 || cy < 0 || cx >= this->get_width() || cy >= this->get_height()) {
    return false;
  }

  // Do we care to check for existing players?
  if (players_are_blocking) {
    // Is there a player at the given cell?
    Player *other = this->get_player_at(cx, cy);
    if (other != nullptr) {
      return false;
    }
  }

  // OK we're good.
  return true;
}

void Game::tick(const GameFrame &game_frame)
{
  std::cout << "Players: Got " << game_frame.get_player_count() << ", Expected " << this->get_player_count() << std::endl;
  assert(game_frame.get_player_count() == this->get_player_count());

  for (int i = 0; i < this->get_player_count(); i++) {
    this->player_set_all_inputs(i,
      game_frame.player_get_all_inputs(i));
  }

  for (Player &p : m_players) {
    p.tick();
  }
}

void Game::draw(void)
{
  for (Player &p : m_players) {
    p.draw();
  }
}

void Game::load_this(istream &ips)
{
  uint16_t player_count = 0;
  load(ips, player_count);
  m_players.clear();

  for (uint16_t i = 0; i < player_count; i++) {
    m_players.push_back(Player(this, ips));
  }
}

void Game::save_this(ostream &ops)
{
  int raw_player_count = this->get_player_count();
  assert(raw_player_count >= 0 && raw_player_count <= 0xFFFF);
  uint16_t player_count = (uint16_t)raw_player_count;
  save(ops, player_count);

  for (uint16_t i = 0; i < player_count; i++) {
    save(ops, m_players[i]);
  }
}

Player *Game::get_player_at(int cx, int cy)
{
  for (Player &p : m_players) {
    if (p.get_x() == cx && p.get_y() == cy) {
      return &p;
    }
  }

  return nullptr;
}

GameFrame::GameFrame(int player_count)
{
  for (int i = 0; i < player_count; i++) {
    m_player_inputs.push_back(PlayerInput());
  }
}
GameFrame::GameFrame(std::istream &ips)
{
  load(ips, *this);
}

void GameFrame::load_this(istream &ips)
{
  uint16_t player_count = 0;
  load(ips, player_count);
  m_player_inputs.clear();

  for (uint16_t i = 0; i < player_count; i++) {
    m_player_inputs.push_back(PlayerInput(ips));
  }
}

void GameFrame::save_this(ostream &ops)
{
  int raw_player_count = this->get_player_count();
  assert(raw_player_count >= 0 && raw_player_count <= 0xFFFF);
  uint16_t player_count = (uint16_t)raw_player_count;
  save(ops, player_count);

  for (uint16_t i = 0; i < player_count; i++) {
    save(ops, m_player_inputs[i]);
  }
}
