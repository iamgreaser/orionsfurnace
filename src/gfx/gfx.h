#ifndef _GFX_GFX_H
#define _GFX_GFX_H

#include <allegro.h>

namespace gfx
{
	extern BITMAP *backbuf;
	extern BITMAP *player_gfx[4];

	void init(void);

	void flip(void);
}

using gfx::backbuf;

#endif /* if !defined(_GFX_GFX_H) */

