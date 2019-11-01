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
#include "core/world.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"

#include <cstdint>
#include <iostream>
using std::istream;
using std::ostream;
#include <vector>
using std::vector;

#if 0
// FIXME: How do you instantiate these again? --GM
template <> void load<uint16_t, Player>(std::istream &ips, std::map<uint16_t, Player> &obj)
template <> void save<uint16_t, Player>(std::ostream &ops, const std::map<uint16_t, Player> &obj);
template <> void load<uint16_t, PlayerInput>(std::istream &ips, std::map<uint16_t, PlayerInput> &obj)
template <> void save<uint16_t, PlayerInput>(std::ostream &ops, const std::map<uint16_t, PlayerInput> &obj);
#endif

namespace game
{
}

Game::Game(void)
{
  m_world = std::make_shared<World>(32, 32);

  World *world = m_world.get();

  int width = world->get_width();
  int height = world->get_height();
  for (uint16_t y = 0; y < height; y++) {
    for (uint16_t x = 0; x < width; x++) {
      if (m_random.next_int(100) < 10) {
        *(world->cell_type_at(x, y)) = cell_type::WALL;
      }
    }
  }
}

Game::Game(std::istream &ips)
{
  load(ips, *this);
}

Game::~Game(void)
{
}

void Game::add_player(int player_idx, Player player)
{
  assert(player_idx >= 0 && player_idx < 0xFFFF);
  assert(!m_players.contains(static_cast<uint16_t>(player_idx)));
  uint16_t key = static_cast<uint16_t>(player_idx);
  m_players.emplace(key, player);
}

void Game::spawn_player(int player_idx)
{
  // FIXME: accept any slot
  int expected_player_idx = static_cast<int>(m_players.size());
  assert(expected_player_idx == player_idx);
  (void)expected_player_idx; // Shut the compiler up
  (void)player_idx; // Shut the compiler up

  // TODO: Add spawn points
  // FIXME: This COULD spawn one player atop another, or atop a wall or something!
  // (but at least it terminates)
  int cx = static_cast<int>(this->random().next_int(
    static_cast<uint32_t>(this->get_width())));
  int cy = static_cast<int>(this->random().next_int(
    static_cast<uint32_t>(this->get_height())));
  for (int i = 0; i < 100; i++) {
    if (this->can_step_into(cx, cy, true)) {
      // Our position is good!
      break;
    }
    cx = static_cast<int>(this->random().next_int(
      static_cast<uint32_t>(this->get_width())));
    cy = static_cast<int>(this->random().next_int(
      static_cast<uint32_t>(this->get_height())));
  }

  // Add the player
  this->add_player(player_idx,
    Player(this, cx, cy, direction::SOUTH));
}

void Game::player_set_all_inputs(int player_idx, PlayerInput player_input)
{
  assert(player_idx >= 0 && player_idx < 0xFFFF);
  m_players.at(static_cast<uint16_t>(player_idx)).set_all_inputs(player_input);
}

bool Game::can_step_into(int cx, int cy, bool players_are_blocking)
{
  // Is this in range of the map?
  if (cx < 0 || cy < 0 || cx >= this->get_width() || cy >= this->get_height()) {
    return false;
  }

  // Is this solid?
  switch (*m_world.get()->cell_type_at(cx, cy)) {
    case cell_type::SPACE:
    case cell_type::FLOOR:
      break;

    case cell_type::WALL:
      return false;

    default:
      return true;
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
  // TODO: Also remove players

  for (int i = 0; i < game_frame.get_player_count(); i++) {
    if (this->get_player_ptr(i) == nullptr) {
      this->spawn_player(i);
    }
  }

  //std::cout << "Players: Got " << game_frame.get_player_count() << ", Expected " << this->get_player_count() << std::endl;
  assert(game_frame.get_player_count() == this->get_player_count());

  for (int i = 0; i < this->get_player_count(); i++) {
    this->player_set_all_inputs(i,
      game_frame.player_get_all_inputs(i));
  }

  for (std::pair<uint16_t, Player> pair : m_players) {
    m_players.at(pair.first).tick();
  }
}

void Game::draw(void)
{
  // Calculate relative cell offset
  // TODO: find a consistent way of biasing against truncation division
  int rel_px = gfx::cam_px;
  int rel_py = gfx::cam_py;
  int rel_cx = (rel_px < 0 ? (rel_px-(24-1))/24 : rel_px/24);
  int rel_cy = (rel_py < 0 ? (rel_py-(24-1))/24 : rel_py/24);
  int width = this->get_width();
  int height = this->get_height();

  // Draw tiles
  // inclusive min, exclusive max
  int cx_imin = 0 + rel_cx;
  int cy_imin = 0 + rel_cy;
  int cx_emax = (CAM_W_CELLS+1) + rel_cx;
  int cy_emax = (CAM_H_CELLS+1) + rel_cy;
  if (cx_imin < 0) { cx_imin = 0; }
  if (cy_imin < 0) { cy_imin = 0; }
  if (cx_emax > width) { cx_emax = width; }
  if (cy_emax > height) { cy_emax = height; }

  m_world.get()->draw_in_cell_range(
    cx_imin, cy_imin, cx_emax, cy_emax);

  for (std::pair<uint16_t, Player> pair : m_players) {
    m_players.at(pair.first).draw();
  }
}

void Game::load_this(istream &ips)
{
#if 0
  load(ips, m_players);
#else
  // maps require us to instantiate the players with no extra arguments at this stage,
  // and Player requires a Game * argument --GM
  uint16_t count = 0;
  load(ips, count);
  m_players.clear();
  for (uint16_t i = 0; i < count; i++) {
    uint16_t key = 0;
    load(ips, key);
    Player value(this, ips);
    m_players.emplace(key, value);
  }
#endif

  m_world = std::make_shared<World>(ips);

  uint32_t seed = 0;
  load(ips, seed);
  m_random.set_seed(seed);
}

void Game::save_this(ostream &ops) const
{
  save(ops, m_players);

  save(ops, *m_world.get());

  uint32_t seed = m_random.get_seed();
  save(ops, seed);
}

Player *Game::get_player_at(int cx, int cy)
{
  for (std::pair<uint16_t, Player> pair : m_players) {
    if (pair.second.get_x() == cx && pair.second.get_y() == cy) {
      return &m_players.at(pair.first);
    }
  }

  return nullptr;
}

GameFrame::GameFrame(void)
{
}

GameFrame::GameFrame(std::istream &ips)
{
  load(ips, *this);
}

void GameFrame::load_this(istream &ips)
{
  load(ips, m_player_inputs);
#if 0
  uint16_t player_count = 0;
  load(ips, player_count);
  m_player_inputs.clear();
  for (uint16_t i = 0; i < player_count; i++) {
    m_player_inputs.push_back(PlayerInput(ips));
  }
#endif
}

void GameFrame::save_this(ostream &ops) const
{
  save(ops, m_player_inputs);
#if 0
  int raw_player_count = this->get_player_count();
  assert(raw_player_count >= 0 && raw_player_count <= 0xFFFF);
  uint16_t player_count = static_cast<uint16_t>(raw_player_count);
  save(ops, player_count);
  for (uint16_t i = 0; i < player_count; i++) {
    save(ops, m_player_inputs[i]);
  }
#endif
}
