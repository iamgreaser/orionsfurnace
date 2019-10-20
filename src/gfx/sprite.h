#ifndef _GFX_SPRITE_H
#define _GFX_SPRITE_H

#include <string>

namespace gfx
{
	using std::string;

	class Sprite
	{
	private:
		const string m_filename;
		void *m_loaded_sprite = NULL;
		int m_xtiles;
		int m_ytiles;
	public:
		Sprite(const string filename, int xtiles, int ytiles);
		~Sprite();

		void ensure_loaded(void);

		void draw(int tx, int ty, int px, int py);
	};

	extern Sprite tile_gfx_floor;
	extern Sprite player_gfx;
}

using gfx::Sprite;

#endif /* if !defined(_GFX_SPRITE_H) */
