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

#include "core/core.h"
#include "core/helpers.h"
#include "core/save.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

namespace world
{
  template <class T> class Layer
  {
  protected:
    uint16_t m_width;
    uint16_t m_height;
    T *m_body = nullptr;
  public:
    Layer(uint16_t width, uint16_t height, T default_value)
      : m_width(width)
      , m_height(height)
    {
      size_t elem_count = 1;
      assert(m_width >= 1);
      assert(m_height >= 1);
      elem_count *= static_cast<size_t>(m_width);
      elem_count *= static_cast<size_t>(m_height);

      m_body = new T[elem_count];
      for (size_t i = 0; i < width*height; i++) {
        m_body[i] = default_value;
      }
    };

    ~Layer(void)
    {
      delete[] m_body;
    }

    const T *at_const(int x, int y) const
    {
      if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        PANIC("at() called with out of range x, y values");
      }

      return &m_body[y * m_width + x];
    }

    T *at(int x, int y)
    {
      if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        PANIC("at() called with out of range x, y values");
      }

      return &m_body[y * m_width + x];
    }
  };

  class World final : public Saveable
  {
  private:
    uint16_t m_width;
    uint16_t m_height;
    std::shared_ptr<Layer<CellType>> m_cell_types;

  public:
    World(int width, int height);
    World(std::istream &ips);
    uint16_t get_width(void) { return m_width; }
    uint16_t get_height(void) { return m_height; }
    void load_this(std::istream &ips);
    void save_this(std::ostream &ops) const;

    void draw_in_cell_range(
      int cx_imin, int cy_imin, int cx_emax, int cy_emax);
  };
}

#endif /* if !defined(CORE_WORLD_H) */
