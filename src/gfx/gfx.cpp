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

#include "gfx/gfx.h"

#include "gfx/font.h"
#include "gfx/sprite.h"

#include "gfx/internal.h"

#include "core/version.h"

#include <SDL.h>

#include <cassert>
#include <string>

namespace gfx
{
  // 1x = 640x360
  // 2x = 1280x720
  // 3x = 1920x1080
  constexpr int INITIAL_SCREEN_SCALE = 2;
  int current_window_width = BASE_SCREEN_WIDTH*INITIAL_SCREEN_SCALE;
  int current_window_height = BASE_SCREEN_HEIGHT*INITIAL_SCREEN_SCALE;

  int cam_px = 0;
  int cam_py = 0;

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *backbuf = nullptr;
}

void gfx::init(bool use_software_rendering)
{
  // Create a window
  std::string window_title = (
    std::string("orion's furnace v")
    + get_engine_version());
  window = SDL_CreateWindow(
    window_title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    current_window_width,
    current_window_height,
    (0
      //| SDL_WINDOW_RESIZABLE;
      ));
  assert(window != nullptr);

  // Create a renderer for the window
  int renderer_flags = (0
    | SDL_RENDERER_PRESENTVSYNC
    | SDL_RENDERER_TARGETTEXTURE);

  if (use_software_rendering) {
    renderer_flags &= ~SDL_RENDERER_PRESENTVSYNC;
    renderer_flags |= SDL_RENDERER_SOFTWARE;
  }
  renderer = SDL_CreateRenderer(window, -1, renderer_flags);
  assert(renderer != nullptr);

  // Create our backbuffer
  backbuf = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_BGRA8888,
    SDL_TEXTUREACCESS_TARGET,
    BASE_SCREEN_WIDTH,
    BASE_SCREEN_HEIGHT);
  assert(backbuf != nullptr);

  // Target the backbuffer by default
  int did_set_render_target = SDL_SetRenderTarget(
    renderer, backbuf);
  assert(did_set_render_target == 0);

  // Initialise SDL_ttf
  init_font();

  // Load some assets
  tile_gfx_floor.ensure_loaded();
  player_gfx.ensure_loaded();
}

void gfx::clip_nothing(void)
{
  // Clear clipping rectangle
  int did_set_clip_rect = SDL_RenderSetClipRect(renderer, nullptr);
  assert(did_set_clip_rect == 0);
}

void gfx::clip_rect(int px, int py, int pw, int ph)
{
  // Set clipping rectangle
  SDL_Rect rect = {};
  rect.x = px;
  rect.y = py;
  rect.w = pw;
  rect.h = ph;

  int did_set_clip_rect = SDL_RenderSetClipRect(renderer, &rect);
  assert(did_set_clip_rect == 0);
}

void gfx::set_camera(int px, int py)
{
  cam_px = px;
  cam_py = py;
}

void gfx::clear(int r, int g, int b)
{
  // Clear screen
  SDL_SetRenderDrawColor(renderer,
    (r < 0 ? 0 : r > 0xFF ? 0xFF : static_cast<uint8_t>(r)),
    (g < 0 ? 0 : g > 0xFF ? 0xFF : static_cast<uint8_t>(g)),
    (b < 0 ? 0 : b > 0xFF ? 0xFF : static_cast<uint8_t>(b)),
    0xFF);
  SDL_RenderFillRect(renderer, nullptr);
}

void gfx::draw_rect(int px, int py, int pw, int ph, int r, int g, int b)
{
  SDL_Rect rect = {};
  rect.x = px - cam_px;
  rect.y = py - cam_py;
  rect.w = pw;
  rect.h = ph;

  SDL_SetRenderDrawColor(renderer,
    (r < 0 ? 0 : r > 0xFF ? 0xFF : static_cast<uint8_t>(r)),
    (g < 0 ? 0 : g > 0xFF ? 0xFF : static_cast<uint8_t>(g)),
    (b < 0 ? 0 : b > 0xFF ? 0xFF : static_cast<uint8_t>(b)),
    0xFF);
  int did_rect = SDL_RenderFillRect(renderer, &rect);
  assert(did_rect == 0);
}

void gfx::draw_text(int px, int py, int r, int g, int b, const std::string text)
{
  font_base.draw(px - cam_px, py - cam_py, r, g, b, text);
}

void gfx::fetch_text_dims(std::string s, int *px, int *py)
{
  font_base.fetch_dims_of(s, px, py);
}

void gfx::flip(void)
{
  // Clear render target back to default
  int did_clear_render_target = SDL_SetRenderTarget(
    renderer, nullptr);
  assert(did_clear_render_target == 0);

  // Blit
  int did_blit = SDL_RenderCopy(
    renderer, backbuf, nullptr, nullptr);
  assert(did_blit == 0);

  // Present
  SDL_RenderPresent(renderer);

  // Set render target again
  int did_set_render_target = SDL_SetRenderTarget(
    renderer, backbuf);
  assert(did_set_render_target == 0);
}
