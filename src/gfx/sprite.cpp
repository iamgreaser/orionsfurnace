/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "gfx/sprite.h"

#include "gfx/gfx.h"

#include "gfx/internal.h"

#include <SDL.h>
#include <SDL_image.h>

#include <cassert>
#include <iostream>
#include <string>

namespace gfx
{
	Sprite tile_gfx_floor("dat/gfx/tiles/floor001.png");
	Sprite player_gfx("dat/gfx/player/base.png");
}

Sprite::Sprite(const string filename, int pw, int ph)
	: m_filename(filename)
	, m_pw(pw)
	, m_ph(ph)
{
}

Sprite::~Sprite()
{
	if (m_loaded_texture != NULL) {
		SDL_Texture *texture = static_cast<SDL_Texture *>(m_loaded_texture);
		SDL_DestroyTexture(texture);
		m_loaded_texture = NULL;
	}
}

void Sprite::ensure_loaded(void)
{
	if (m_loaded_texture == NULL) {
		// Load image
		SDL_Surface *surface = IMG_Load(m_filename.c_str());
		if (surface == NULL)
		{
			std::cerr << "SDL ERROR: Attempting to load \"" << m_filename << "\": \"" << SDL_GetError() << "\"" << std::endl;
			std::cerr.flush();
		}
		assert(surface != NULL);

		// Create texture
		SDL_Texture *texture = SDL_CreateTextureFromSurface(
			renderer, surface);
		assert(texture != NULL);
		m_loaded_texture = static_cast<void *>(texture);

		// Free surface as per the SDL manual recommendations
		// This should save... a little bit of RAM?
		SDL_FreeSurface(surface);
	}
}

void Sprite::draw(int px, int py, int tx, int ty)
{
	this->ensure_loaded();

	// Build blit rectangles
	SDL_Rect srcrect = {};
	SDL_Rect dstrect = {};
	srcrect.x = tx*m_pw;
	srcrect.y = ty*m_ph;
	srcrect.w = m_pw;
	srcrect.h = m_ph;
	dstrect.x = px;
	dstrect.y = py;
	dstrect.w = m_pw;
	dstrect.h = m_ph;

	// Now use them to actually blit
	SDL_Texture *texture = static_cast<SDL_Texture *>(m_loaded_texture);
	int did_blit = SDL_RenderCopy(
		renderer, texture, &srcrect, &dstrect);
	assert(did_blit == 0);
}
