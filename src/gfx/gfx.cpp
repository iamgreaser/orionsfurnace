#include "gfx/gfx.h"

#include <allegro.h>

#include <cassert>

namespace gfx
{
	// 1x = 320x200
	// 2x = 640x400
	// 3x = 960x600
	// 4x = 1280x800
	// 5x = 1600x1000
	constexpr int BASE_SCREEN_WIDTH = 320;
	constexpr int BASE_SCREEN_HEIGHT = 200;
	constexpr int INITIAL_SCREEN_SCALE = 4;
	int current_window_width = BASE_SCREEN_WIDTH*INITIAL_SCREEN_SCALE;
	int current_window_height = BASE_SCREEN_HEIGHT*INITIAL_SCREEN_SCALE;

	BITMAP *backbuf = NULL;
}

void gfx::init(void)
{
	set_color_depth(32);
	set_gfx_mode(
		GFX_AUTODETECT_WINDOWED,
		current_window_width,
		current_window_height,
		0,
		0);
	backbuf = create_bitmap_ex(
		32,
		BASE_SCREEN_WIDTH,
		BASE_SCREEN_HEIGHT);
	assert(backbuf != NULL);
}

void gfx::flip(void)
{
	stretch_blit(
		backbuf, screen,
		0, 0, backbuf->w, backbuf->h,
		0, 0, screen->w, screen->h);
}
