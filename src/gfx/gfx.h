#ifndef _GFX_GFX_H
#define _GFX_GFX_H

#include <allegro.h>

// TODO: move to a core lib

namespace direction
{
	enum Direction {
		NORTH = 0,
		SOUTH = 1,
		WEST  = 2,
		EAST  = 3,
	};
}
using direction::Direction;

namespace gfx
{
	extern BITMAP *backbuf;
	extern BITMAP *player_gfx[4];

	void init(void);

	void flip(void);
}

using gfx::backbuf;

#endif /* if !defined(_GFX_GFX_H) */

