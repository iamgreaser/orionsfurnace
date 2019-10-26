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

#ifndef GFX_INTERNAL_H
#define GFX_INTERNAL_H

#include <SDL.h>

namespace gfx
{
  extern SDL_Window *window;
  extern SDL_Renderer *renderer;
  extern SDL_Texture *backbuf;

  void init_font(void);
}
using gfx::backbuf;

#endif /* if !defined(GFX_INTERNAL_H) */
