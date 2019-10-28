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

#ifndef GFX_GFX_H
#define GFX_GFX_H

#include <string>

namespace gfx
{
  constexpr int BASE_SCREEN_WIDTH = 640;
  constexpr int BASE_SCREEN_HEIGHT = 360;
  constexpr int CELL_W = 24;
  constexpr int CELL_H = 24;
  constexpr int CAM_X = 0;
  constexpr int CAM_Y = 0;
  constexpr int CAM_W_CELLS = 15;
  constexpr int CAM_H_CELLS = 15;
  constexpr int CAM_W = CELL_W*CAM_W_CELLS;
  constexpr int CAM_H = CELL_H*CAM_H_CELLS;

  constexpr int SIDEBAR_X = CAM_W;
  constexpr int SIDEBAR_Y = 0;
  constexpr int SIDEBAR_W = BASE_SCREEN_WIDTH-SIDEBAR_X;
  constexpr int SIDEBAR_H = BASE_SCREEN_HEIGHT;

  extern int cam_px;
  extern int cam_py;

  void init(bool use_software_rendering);

  void clip_nothing(void);
  void clip_rect(int px, int py, int pw, int ph);

  void set_camera(int px, int py);

  void clear(int r, int g, int b);
  void draw_rect(int px, int py, int pw, int ph, int r, int g, int b);
  void draw_text(int px, int py, int r, int g, int b, const std::string text);
  void fetch_text_dims(std::string s, int *px, int *py);

  void flip(void);
}

using gfx::CELL_W;
using gfx::CELL_H;
using gfx::CAM_X;
using gfx::CAM_Y;
using gfx::CAM_W_CELLS;
using gfx::CAM_H_CELLS;
using gfx::CAM_W;
using gfx::CAM_H;
using gfx::SIDEBAR_X;
using gfx::SIDEBAR_Y;
using gfx::SIDEBAR_W;
using gfx::SIDEBAR_H;

#endif /* if !defined(GFX_GFX_H) */
