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

#include "core/world.h"

#include "gfx/gfx.h"
#include "gfx/sprite.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>

World::World(uint16_t width, uint16_t height)
  : m_width(width)
  , m_height(height)
  , m_cell_types(std::make_shared<Layer<CellType>>(width, height, cell_type::FLOOR))
{
  assert(m_width >= 1);
  assert(m_height >= 1);
}

World::World(std::istream &ips)
{
  load(ips, *this);
}

void World::load_this(std::istream &ips)
{
  load(ips, m_width);
  load(ips, m_height);
  assert(m_width >= 1);
  assert(m_height >= 1);

  m_cell_types = std::make_shared<Layer<CellType>>(m_width, m_height, cell_type::SPACE);

  for (uint16_t y = 0; y < m_height; y++) {
    for (uint16_t x = 0; x < m_height; x++) {
      // NOTE: Interleave these here
      uint16_t cell_type_ui = 0;
      load(ips, cell_type_ui);
      assert (cell_type_ui < 3);
      *(m_cell_types.get()->at(x, y)) = static_cast<CellType>(cell_type_ui);
    }
  }
}

void World::save_this(std::ostream &ops) const
{
  save(ops, m_width);
  save(ops, m_height);

  for (uint16_t y = 0; y < m_height; y++) {
    for (uint16_t x = 0; x < m_height; x++) {
      // NOTE: Interleave these here
      uint16_t cell_type_ui = static_cast<uint16_t>(
        *(m_cell_types.get()->at_const(x, y)));
      save(ops, cell_type_ui);
    }
  }
}

void World::draw_in_cell_range(
  int cx_imin, int cy_imin, int cx_emax, int cy_emax)
{
  Layer<CellType> *cell_types = m_cell_types.get();

  for (int cy = cy_imin; cy < cy_emax; cy++) {
    int py = CAM_Y + (cy*CELL_H);

    for (int cx = cx_imin; cx < cx_emax; cx++) {
      int px = CAM_X + (cx*CELL_W);

      switch (*(cell_types->at(cx, cy))) {
        case cell_type::SPACE:
          // TODO: space tile?
          break;

        case cell_type::FLOOR:
          gfx::tile_gfx_floor.draw(px, py);
          break;

        //case cell_type::WALL:
        //  break;

        default:
          PANIC("Unhandled tile type");
          break;
      }
    }
  }
}
