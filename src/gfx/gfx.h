#ifndef _GFX_GFX_H
#define _GFX_GFX_H

#include <allegro.h>

namespace gfx
{
	extern BITMAP *backbuf;

	void init(void);

	void flip(void);
}

#endif /* if !defined(_GFX_GFX_H) */

