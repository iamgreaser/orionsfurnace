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

#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include "core/core.h"

#include "core/save.h"

#include <cassert>
#include <cstdint>
#include <iostream>

class PlayerInput : public Saveable
{
private:
  bool m_input_move[4] = {};
public:
  PlayerInput(void);
  PlayerInput(std::istream &ips);

  bool get_input_move(Direction dir) {
    assert(dir >= 0 && dir < 4);
    return m_input_move[dir];
  }
  void set_input_move(Direction dir, bool v) {
    assert(dir >= 0 && dir < 4);
    m_input_move[dir] = v;
  }

  void load_this(std::istream &ips) override;
  void save_this(std::ostream &ops) override;
};

namespace diagonal_fixer
{
  enum DiagonalFixer
  {
    // The next diagonal should be vertical.
    NEXT_VERTICAL = 0,

    // The next diagonal should be horizontal.
    NEXT_HORIZONTAL,

    // There is an active diagonal going horizontal.
    ASSERTING_HORIZONTAL,

    // There is an active diagonal going vertical.
    ASSERTING_VERTICAL,
  };
}
using diagonal_fixer::DiagonalFixer;

class Player : public Saveable
{
  friend class Game; // m_game MUST point to the correct game!
private:
  Game *m_game;
  int32_t m_cx;
  int32_t m_cy;
  Direction m_dir;
  DiagonalFixer m_diagonal_fixer;

  int32_t m_pos_interp_x0;
  int32_t m_pos_interp_y0;
  int32_t m_pos_interp_x1;
  int32_t m_pos_interp_y1;
  int32_t m_pos_interp_remain = 0;
  int32_t m_pos_interp_len = 0;

  PlayerInput m_input;
  int32_t m_input_cooldown = 0;

public:
  Player(Game *game, int cx, int cy, Direction dir);
  Player(Game *game, std::istream &ips);
  Player(void);

  int get_x(void) { return m_cx; }
  int get_y(void) { return m_cy; }
  Direction get_dir(void) { return m_dir; }
  bool get_input_move(Direction dir) {
    return m_input.get_input_move(dir);
  }
  void set_input_move(Direction dir, bool v) {
    m_input.set_input_move(dir, v);
  }
  void set_all_inputs(PlayerInput player_input);
  void set_game(Game *game) {
    // FIXME THIS IS A KLUDGE
    // - I may need to pass a Game * argument around for things?
    m_game = game;
  }

  void tick(void);

  void draw(void);

  void load_this(std::istream &ips) override;
  void save_this(std::ostream &ops) override;

  bool attempt_move_by(int dx, int dy);
  bool attempt_move_to(int cx, int cy);

protected:
  void calc_interp_pos(int *px, int *py);
};

class PlayerAdd : public Saveable
{
private:
  uint16_t m_player_idx;
  Player m_player;
public:
  PlayerAdd(int player_idx, Player player);
  PlayerAdd(std::istream &ips);

  Player get_player(void) { return m_player; }
  int get_player_idx(void) { return m_player_idx; }

  void load_this(std::istream &ips) override;
  void save_this(std::ostream &ops) override;
};

#endif /* if !defined(CORE_PLAYER_H) */
