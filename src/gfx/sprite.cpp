#include "gfx/sprite.h"

#include "gfx/gfx.h"

#include <allegro.h>

#include <cassert>
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
	if (m_loaded_sprite != NULL) {
		// FIXME: For some reason, Allegro seems to free these when done.
		// Once we move to SDL 2, reinstate this as its SDL equivalent. --GM

		//std::cout << "Destroying bitmap " << m_loaded_sprite << std::endl;
		//BITMAP *bmp = static_cast<BITMAP *>(m_loaded_sprite);
		//destroy_bitmap(bmp);

		m_loaded_sprite = NULL;
	}
}

void Sprite::ensure_loaded(void)
{
	if (m_loaded_sprite == NULL) {
		BITMAP *bmp = load_bitmap(m_filename.c_str(), NULL);
		assert(bmp != NULL);
		m_loaded_sprite = static_cast<void *>(bmp);
	}
}

void Sprite::draw(int px, int py, int tx, int ty)
{
	this->ensure_loaded();

	BITMAP *bmp = static_cast<BITMAP *>(m_loaded_sprite);
	masked_blit(bmp, backbuf,
		tx*m_pw, ty*m_ph,
		px, py,
		m_pw, m_ph);
}
