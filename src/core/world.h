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

#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include "core/helpers.h"
#include "core/save.h"

#include <cstdint>
#include <iostream>
#include <vector>

namespace world
{
  namespace cell_type
  {
    enum CellType {
      SPACE = 0,
      FLOOR,
      WALL,
    };
  }
  using cell_type::CellType;

  template <class T, T default_value> class Layer
  {
  protected:
    uint16_t m_width;
    uint16_t m_height;
    T *m_body = nullptr;
  public:
    Layer(uint16_t width, uint16_t height)
      : m_width(width)
      , m_height(height)
      , m_body(new T[width*height])
    {
      for (size_t i = 0; i < width*height; i++) {
        m_body[i] = default_value;
      }
    };

    ~Layer(void)
    {
      delete[] m_body;
    }

    T &at(int x, int y)
    {
      if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        PANIC("at() called with out of range x, y values");
      }
    }
  };

  class World final : public Saveable
  {
  private:
    Layer<CellType, cell_type::SPACE> m_cell_types;

  public:
    World(int width, int height);
    void load_this(std::istream &ips);
    void save_this(std::ostream &ops) const;
  };
}
using world::World;

#endif /* if !defined(CORE_WORLD_H) */
