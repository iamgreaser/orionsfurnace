#include "gfx/sprite.h"

#include "gfx/gfx.h"

#include <allegro.h>

#include <cassert>
#include <string>

namespace gfx
{
	Sprite tile_gfx_floor("dat/gfx/tiles/floor001.png", 1, 1);
	Sprite player_gfx("dat/gfx/player/base.png", 4, 4);
}

Sprite::Sprite(const string filename, int xtiles, int ytiles)
	: m_filename(filename)
	, m_xtiles(xtiles)
	, m_ytiles(ytiles)
{
}

Sprite::~Sprite()
{
	if (m_loaded_sprite != NULL) {
		BITMAP *bmp = static_cast<BITMAP *>(m_loaded_sprite);
		destroy_bitmap(bmp);
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

void Sprite::draw(int tx, int ty, int px, int py)
{
	ensure_loaded();

	// TODO: use tx, ty
	(void)tx;
	(void)ty;

	BITMAP *bmp = static_cast<BITMAP *>(m_loaded_sprite);
	draw_sprite(backbuf, bmp, px, py);
}
