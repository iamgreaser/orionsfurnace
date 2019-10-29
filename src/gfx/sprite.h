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

#ifndef GFX_SPRITE_H
#define GFX_SPRITE_H

#include <cstddef>
#include <string>

#include "gfx/gfx.h"

namespace gfx
{
  using std::string;

  class Sprite
  {
  private:
    const string m_filename;
    void *m_loaded_texture = nullptr;
    int m_pw;
    int m_ph;
  public:
    Sprite(const string filename, int pw = CELL_W, int ph = CELL_H);
    ~Sprite();

    void ensure_loaded(void);

    void draw(int px, int py, int tx = 0, int ty = 0);
  };

  extern Sprite tile_gfx_floor;
  extern Sprite tile_gfx_wall;
  extern Sprite player_gfx;
}

using gfx::Sprite;

#endif /* if !defined(GFX_SPRITE_H) */
