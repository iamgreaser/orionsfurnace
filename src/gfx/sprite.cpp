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

	if (m_loaded_surface != NULL) {
		SDL_Surface *surface = static_cast<SDL_Surface *>(m_loaded_surface);
		SDL_FreeSurface(surface);
		m_loaded_surface = NULL;
	}
}

void Sprite::ensure_loaded(void)
{
	if (m_loaded_texture == NULL) {
		if (m_loaded_surface == NULL) {
			SDL_Surface *surface = IMG_Load(m_filename.c_str());
			if (surface == NULL)
			{
				std::cerr << "SDL ERROR: Attempting to load \"" << m_filename << "\": \"" << SDL_GetError() << "\"" << std::endl;
				std::cerr.flush();
			}
			assert(surface != NULL);
			m_loaded_surface = static_cast<void *>(surface);
		}
		SDL_Texture *texture = SDL_CreateTextureFromSurface(
			renderer,
			static_cast<SDL_Surface *>(m_loaded_surface));
		assert(texture != NULL);
		m_loaded_texture = static_cast<void *>(texture);
	}
}

void Sprite::draw(int px, int py, int tx, int ty)
{
	this->ensure_loaded();

	// TODO!
	(void)px;
	(void)py;
	(void)tx;
	(void)ty;
}
