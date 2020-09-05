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

#include "core/player.h"

#include "core/core.h"
#include "core/entity.h"
#include "core/helpers.h"
#include "core/game.h"

#include <cassert>
#include <cstdint>
#include <iostream>
using std::istream;
using std::ostream;

Player::Player(Game *game, int cx, int cy, Direction dir)
  : m_game(game)
  , m_entity(game, cx, cy, dir)
{
}

Player::Player(Game *game, std::istream &ips)
  : m_game(game)
  , m_entity(game, 0, 0, direction::SOUTH) // Dummy values
{
  load(ips, *this);
}

void Player::set_all_inputs(PlayerInput player_input)
{
  m_input = player_input;
}

void Player::tick(void)
{
  // Calculate movement direction
  int dx = 0;
  int dy = 0;

  if (this->get_input_move(direction::NORTH)) { dy -= 1; }
  if (this->get_input_move(direction::SOUTH)) { dy += 1; }
  if (this->get_input_move(direction::WEST)) { dx -= 1; }
  if (this->get_input_move(direction::EAST)) { dx += 1; }

  m_entity.set_walk_dir_this_tick(dx, dy);

  m_entity.tick();
}

void Player::draw(void)
{
  m_entity.draw();
}

void Player::load_this(istream &ips)
{
  load(ips, m_entity);
  load(ips, m_input);
}

void Player::save_this(ostream &ops) const
{
  save(ops, m_entity);
  save(ops, m_input);
}

bool Player::attempt_move_by(int dx, int dy)
{
  return m_entity.attempt_move_by(dx, dy);
}

PlayerInput::PlayerInput(void) {}
PlayerInput::PlayerInput(std::istream &ips)
{
  load(ips, *this);
}

void PlayerInput::load_this(std::istream &ips)
{
  uint8_t dirmask = 0;
  load(ips, dirmask);
  for (int i = 0; i < 4; i++) {
    m_input_move[i] = ((dirmask & (1<<i)) != 0);
  }
}

void PlayerInput::save_this(std::ostream &ops) const
{
  uint8_t dirmask = 0;
  for (int i = 0; i < 4; i++) {
    if (m_input_move[i]) {
      assert(i >= 0 && i <= 8);
      dirmask |= static_cast<uint8_t>((1U<<i));
    }
  }
  save(ops, dirmask);
}
