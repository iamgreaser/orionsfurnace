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

#include <cassert>
#include <cstddef>
#include <memory>

World::World(int width, int height)
  : m_width(width)
  , m_height(height)
  , m_cell_types(std::make_shared<Layer<CellType>>(width, height, cell_type::FLOOR))
{
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
      save(ops, *(m_cell_types.get()->at_const(x, y)));
    }
  }
}
