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

#ifndef CORE_ENTITY_H
#define CORE_ENTITY_H

#include "core/core.h"

#include "core/save.h"

#include <cstdint>
#include <iostream>

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

namespace entity
{
  class Entity final : public Saveable
  {
  private:
    Game *m_game;
    int32_t m_cx;
    int32_t m_cy;
    Direction m_dir;
    DiagonalFixer m_diagonal_fixer;

    int32_t m_walk_dx = 0;
    int32_t m_walk_dy = 0;

    int32_t m_pos_interp_x0;
    int32_t m_pos_interp_y0;
    int32_t m_pos_interp_x1;
    int32_t m_pos_interp_y1;
    int32_t m_pos_interp_remain = 0;
    int32_t m_pos_interp_len = 0;

    int32_t m_input_cooldown = 0;

  public:
    Entity(Game *m_game, int32_t cx, int32_t cy, Direction dir);
    Entity(Game *m_game, std::istream &ips);

    int get_x(void) { return m_cx; }
    int get_y(void) { return m_cy; }
    Direction get_dir(void) { return m_dir; }

    void set_walk_dir_this_tick(int dx, int dy);
    void tick(void);

    void draw(void);

    bool attempt_move_by(int dx, int dy);
    bool attempt_move_to(int cx, int cy);

    void calc_interp_pos(int *px, int *py);

    void load_this(std::istream &ips) override;
    void save_this(std::ostream &ops) const override;
  };
}
#endif /* if !defined(CORE_ENTITY_H) */
