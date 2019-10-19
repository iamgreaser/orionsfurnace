#ifndef _GFX_PNG_H
#define _GFX_PNG_H

#include <allegro.h>

namespace gfx
{
	void install_png_loader(void);

	BITMAP *load_png(const char *filename, RGB *pal);
}

#endif /* if !defined(_GFX_PNG_H) */
