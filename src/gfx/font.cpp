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

#include "gfx/font.h"

#include "gfx/gfx.h"

#include "gfx/internal.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <cassert>
#include <iostream>

namespace gfx
{
  Font font_base("dat/fonts/DejaVuSans.ttf", 11);
}

Font::Font(const string filename, int ptsize)
  : m_filename(filename)
  , m_ptsize(ptsize)
{
}

Font::~Font()
{
  if (m_loaded_font != nullptr) {
    TTF_Font *ttf = static_cast<TTF_Font *>(m_loaded_font);
    TTF_CloseFont(ttf);
    m_loaded_font = nullptr;
  }
}

void Font::ensure_loaded(void)
{
  if (m_loaded_font == nullptr) {
    TTF_Font *ttf = TTF_OpenFont(
      m_filename.c_str(), m_ptsize);
    assert(ttf != nullptr);
    m_loaded_font = static_cast<void *>(ttf);
  }
}

void gfx::init_font(void)
{
  int did_init = TTF_Init();
  assert(did_init == 0);
}

void Font::draw(int px, int py, int r, int g, int b, const std::string text)
{
  this->ensure_loaded();

  // Build colour
  SDL_Color fg = {};
  fg.r = (r < 0 ? 0 : r > 0xFF ? 0xFF : (uint8_t)r);
  fg.g = (g < 0 ? 0 : g > 0xFF ? 0xFF : (uint8_t)g);
  fg.b = (b < 0 ? 0 : b > 0xFF ? 0xFF : (uint8_t)b);
  fg.a = 0xFF;

  // Build surface
  TTF_Font *ttf = static_cast<TTF_Font *>(m_loaded_font);
  SDL_Surface *surface = TTF_RenderUTF8_Blended(
    ttf, text.c_str(), fg);
  assert(surface != nullptr);

  // Build blit rectangle
  SDL_Rect dstrect = {};
  dstrect.x = px;
  dstrect.y = py;
  dstrect.w = surface->w;
  dstrect.h = surface->h;

  // Build texture
  SDL_Texture *texture = SDL_CreateTextureFromSurface(
    renderer, surface);
  assert(texture != nullptr);

  // Free surface
  SDL_FreeSurface(surface);

  // Blit text
  int did_blit = SDL_RenderCopy(
    renderer, texture, nullptr, &dstrect);
  assert(did_blit == 0);

  // Destroy texture
  SDL_DestroyTexture(texture);

}

void Font::fetch_dims_of(std::string s, int *pw, int *ph)
{
  this->ensure_loaded();

  TTF_Font *ttf = static_cast<TTF_Font *>(m_loaded_font);
  int did_metrics = TTF_SizeUTF8(ttf, s.c_str(), pw, ph);
  assert(did_metrics == 0);
}
