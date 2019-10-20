#ifndef _GFX_SPRITE_H
#define _GFX_SPRITE_H

#include <string>

#include "gfx/gfx.h"

namespace gfx
{
	using std::string;

	class Sprite
	{
	private:
		const string m_filename;
		void *m_loaded_texture = NULL;
		int m_pw;
		int m_ph;
	public:
		Sprite(const string filename, int pw = CELL_W, int ph = CELL_H);
		~Sprite();

		void ensure_loaded(void);

		void draw(int px, int py, int tx = 0, int ty = 0);
	};

	extern Sprite tile_gfx_floor;
	extern Sprite player_gfx;
}

using gfx::Sprite;

#endif /* if !defined(_GFX_SPRITE_H) */
