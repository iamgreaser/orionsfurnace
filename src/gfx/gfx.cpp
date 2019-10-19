#include "gfx/gfx.h"

#include "gfx/png.h"

#include <allegro.h>

#include <cassert>

namespace gfx
{
	// 1x = 640x360
	// 2x = 1280x720
	// 3x = 1920x1080
	constexpr int BASE_SCREEN_WIDTH = 640;
	constexpr int BASE_SCREEN_HEIGHT = 360;
	constexpr int INITIAL_SCREEN_SCALE = 2;
	int current_window_width = BASE_SCREEN_WIDTH*INITIAL_SCREEN_SCALE;
	int current_window_height = BASE_SCREEN_HEIGHT*INITIAL_SCREEN_SCALE;

	BITMAP *backbuf = NULL;

	BITMAP *player_gfx_base = NULL;
	BITMAP *player_gfx[4] = {};
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

	install_png_loader();

	player_gfx_base = load_bitmap("dat/gfx/player/base.png", NULL);
	assert(player_gfx_base != NULL);

	for(int i = 0; i < 4; i++) {
		player_gfx[i] = create_sub_bitmap(
			player_gfx_base,
			i*24, 0,
			24, 24);
		assert(player_gfx[i] != NULL);
	}
}

void gfx::flip(void)
{
	stretch_blit(
		backbuf, screen,
		0, 0, backbuf->w, backbuf->h,
		0, 0, screen->w, screen->h);
}
