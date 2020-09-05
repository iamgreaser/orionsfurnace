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

#include "core/entity.h"

#include "core/game.h"
#include "core/player.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"

#include <iostream>
using std::istream;
using std::ostream;

Entity::Entity(Game *game, int cx, int cy, Direction dir)
  : m_game(game)
  , m_cx(cx)
  , m_cy(cy)
  , m_dir(dir)
  , m_diagonal_fixer(diagonal_fixer::NEXT_VERTICAL)
  , m_pos_interp_x0(cx*CELL_W)
  , m_pos_interp_y0(cy*CELL_H)
  , m_pos_interp_x1(cx*CELL_W)
  , m_pos_interp_y1(cy*CELL_H)
{
}

Entity::Entity(Game *game, std::istream &ips)
  : m_game(game)
{
  load(ips, *this);
}

void Entity::set_walk_dir_this_tick(int dx, int dy)
{
  assert (dx >= -1 && dx <= +1);
  assert (dy >= -1 && dy <= +1);
  m_walk_dx = dx;
  m_walk_dy = dy;
}

void Entity::tick(void)
{
  // Pop walk dir
  int dx = m_walk_dx;
  int dy = m_walk_dy;
  m_walk_dx = 0;
  m_walk_dy = 0;
  assert (dx >= -1 && dx <= +1);
  assert (dy >= -1 && dy <= +1);

  // Fix diagonals
  if (dx != 0 && dy != 0) {
    switch (m_diagonal_fixer) {
      case diagonal_fixer::NEXT_VERTICAL:
      case diagonal_fixer::ASSERTING_VERTICAL:
        dx = 0;
        m_diagonal_fixer = diagonal_fixer::ASSERTING_VERTICAL;
        break;

      case diagonal_fixer::NEXT_HORIZONTAL:
      case diagonal_fixer::ASSERTING_HORIZONTAL:
        dy = 0;
        m_diagonal_fixer = diagonal_fixer::ASSERTING_HORIZONTAL;
        break;

      default:
        PANIC("LOGIC FAILURE");
        break;
    }

  } else if (dx != 0) {
    assert(dy == 0);
    m_diagonal_fixer = diagonal_fixer::NEXT_VERTICAL;
  } else if (dy != 0) {
    assert(dx == 0);
    m_diagonal_fixer = diagonal_fixer::NEXT_HORIZONTAL;
  } else {
    assert(dx == 0 && dy == 0);

    // Handle idle case sanely.
    // Favour switching here.
    switch (m_diagonal_fixer) {
      case diagonal_fixer::NEXT_VERTICAL:
      case diagonal_fixer::ASSERTING_HORIZONTAL:
        m_diagonal_fixer = diagonal_fixer::NEXT_VERTICAL;
        break;

      case diagonal_fixer::NEXT_HORIZONTAL:
      case diagonal_fixer::ASSERTING_VERTICAL:
        dy = 0;
        m_diagonal_fixer = diagonal_fixer::NEXT_HORIZONTAL;
        break;

      default:
        PANIC("LOGIC FAILURE");
        break;
    }
  }

  // Calculate facing direction base on movement direction
  if (dy < 0) { m_dir = direction::NORTH; }
  if (dy > 0) { m_dir = direction::SOUTH; }
  if (dx < 0) { m_dir = direction::WEST; }
  if (dx > 0) { m_dir = direction::EAST; }

  // Apply input cooldown
  if (m_input_cooldown <= 0) {
    // If we're moving, then move.
    if (dx != 0 || dy != 0) {
      if (this->attempt_move_by(dx, dy)) {
        m_input_cooldown = 12;
      }
    }
  }

  if (m_input_cooldown > 0) {
    m_input_cooldown -= 1;
  }

  // Apply pixel position interpolation
  // TODO: Make a timestamp and do the interp in draw()
  if (m_pos_interp_remain > 0) {
    m_pos_interp_remain -= 1;
    if (m_pos_interp_remain == 0) {
      // We're done with interpolation.
      // Snap to position.
      m_pos_interp_len = 0;
      m_pos_interp_x0 = m_pos_interp_x1;
      m_pos_interp_y0 = m_pos_interp_y1;
    }
  }
}


void Entity::draw(void)
{
  // Calculate position
  int px = m_pos_interp_x1;
  int py = m_pos_interp_y1;
  this->calc_interp_pos(&px, &py);
  gfx::player_gfx.draw(px, py, m_dir, 0);
}

bool Entity::attempt_move_by(int dx, int dy)
{
  int cx = m_cx + dx;
  int cy = m_cy + dy;

  // Can we step into here?
  if (!m_game->can_step_into(cx, cy, false)) {
    return false;
  }

  // Is there a player at the given cell?
  Player *other = m_game->get_player_at(cx, cy);
  if (other != nullptr) {
    // Is it us?
    if (other->get_entity() == this) {
      // Don't push ourselves, that causes stack overflows.
      // I've crashed ZZT enough times to know to do this. --GM
      return false;
    }

    // Can we push it?
    if (!other->attempt_move_by(dx, dy)) {
      return false;
    }
  }

  // Face this direction.
  if (dy < 0) { m_dir = direction::NORTH; }
  if (dy > 0) { m_dir = direction::SOUTH; }
  if (dx < 0) { m_dir = direction::WEST; }
  if (dx > 0) { m_dir = direction::EAST; }

  // Move to the next cell.
  return this->attempt_move_to(cx, cy);
}

bool Entity::attempt_move_to(int cx, int cy)
{
  // Can we step into here?
  if (!m_game->can_step_into(cx, cy, true)) {
    return false;
  }

  // Move.
  m_cx = cx;
  m_cy = cy;

  // Interpolate.
  int new_px = m_pos_interp_x1;
  int new_py = m_pos_interp_y1;
  this->calc_interp_pos(&new_px, &new_py);
  m_pos_interp_x0 = new_px;
  m_pos_interp_y0 = new_py;
  m_pos_interp_x1 = m_cx*CELL_W;
  m_pos_interp_y1 = m_cy*CELL_H;
  m_pos_interp_remain = 12+4;
  m_pos_interp_len = m_pos_interp_remain;

  // Acknowledge.
  return true;
}

void Entity::calc_interp_pos(int *px, int *py)
{
  // Calculate position
  *px = m_pos_interp_x1;
  *py = m_pos_interp_y1;

  if (m_pos_interp_remain > 0) {
    int dpx = (m_pos_interp_x1 - m_pos_interp_x0);
    int dpy = (m_pos_interp_y1 - m_pos_interp_y0);
    int t = m_pos_interp_remain;
    int l = m_pos_interp_len;
    *px -= (2*dpx*t+l)/(2*l);
    *py -= (2*dpy*t+l)/(2*l);
  }
}

void Entity::load_this(istream &ips)
{
  load(ips, m_cx);
  load(ips, m_cy);

  uint8_t ui_dir = 0;
  load(ips, ui_dir);
  assert(ui_dir < 4);
  m_dir = static_cast<Direction>(ui_dir);

  uint8_t diagonal = 0;
  load(ips, diagonal);
  assert(diagonal < 4);
  m_diagonal_fixer = static_cast<DiagonalFixer>(diagonal);

  load(ips, m_pos_interp_x0);
  load(ips, m_pos_interp_y0);
  load(ips, m_pos_interp_x1);
  load(ips, m_pos_interp_y1);
  load(ips, m_pos_interp_remain);
  load(ips, m_pos_interp_len);
  load(ips, m_input_cooldown);
}

void Entity::save_this(ostream &ops) const
{
  save(ops, m_cx);
  save(ops, m_cy);

  uint8_t ui_dir = static_cast<uint8_t>(m_dir);
  save(ops, ui_dir);

  uint8_t diagonal = static_cast<uint8_t>(m_diagonal_fixer);
  save(ops, diagonal);

  save(ops, m_pos_interp_x0);
  save(ops, m_pos_interp_y0);
  save(ops, m_pos_interp_x1);
  save(ops, m_pos_interp_y1);
  save(ops, m_pos_interp_remain);
  save(ops, m_pos_interp_len);
  save(ops, m_input_cooldown);
}

