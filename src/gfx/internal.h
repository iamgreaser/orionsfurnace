#ifndef _GFX_INTERNAL_H
#define _GFX_INTERNAL_H

#include <SDL.h>

namespace gfx
{
	extern SDL_Window *window;
	extern SDL_Renderer *renderer;
	extern SDL_Texture *backbuf;
}
using gfx::backbuf;

#endif /* if !defined(_GFX_INTERNAL_H) */
