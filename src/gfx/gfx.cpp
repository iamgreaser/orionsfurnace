#include "gfx/gfx.h"

#include "gfx/png.h"

#include <allegro.h>

#include <cassert>

namespace gfx
{
	// 1x = 640x360
	// 2x = 1280x720
	// 3x = 1920x1080
	constexpr int INITIAL_SCREEN_SCALE = 2;
	int current_window_width = BASE_SCREEN_WIDTH*INITIAL_SCREEN_SCALE;
	int current_window_height = BASE_SCREEN_HEIGHT*INITIAL_SCREEN_SCALE;

	BITMAP *backbuf = NULL;

#if 0
	BITMAP *vidbufs[2] = {};
	int vidbuf_in = 0;
	int vidbuf_out = 1;
#endif

	BITMAP *player_gfx_base = NULL;
	BITMAP *player_gfx[4] = {};
}

void gfx::init(void)
{
	// Set our mode
	set_color_depth(32);
	set_display_switch_mode(SWITCH_BACKAMNESIA);
	set_gfx_mode(
		GFX_AUTODETECT_WINDOWED,
		current_window_width,
		current_window_height,
		0,
		0);

	// Install a PNG loader BEFORE attempting to load any images
	install_png_loader();

	// Create our backbuffer
	backbuf = create_bitmap_ex(
		32,
		BASE_SCREEN_WIDTH,
		BASE_SCREEN_HEIGHT);
	assert(backbuf != NULL);

	// TODO: Move away from Allegro 4.
	//
	// On My Machine(tm), I get tearing.
	// - It's not possible to enable triple-buffering.
	// - It's not possible to allocate video bitmaps for page flipping.
	// - V-Sync doesn't actually work.
	//
	// I would have liked to have messed w/ the Allegro GUI.
	// For the replacement I think I'll need to either:
	// - find a nice GUI widget kit, or
	// - roll my own.
	//
	// As for font rendering? SDL_ttf exists.
	// The world can do with more DejaVu Sans.
	// --GM
#if 0
	// Create our video buffers
	for (int i = 0; i < 2; i++) {
		vidbufs[i] = create_video_bitmap(SCREEN_W, SCREEN_H);
		assert(vidbufs[i] != NULL);
	}
#endif

	// Load some assets
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
	// Blit
	stretch_blit(
		backbuf,

		screen,
		//vidbufs[vidbuf_in],

		0, 0, backbuf->w, backbuf->h,
		0, 0, screen->w, screen->h);

#if 0
	// Swap buffers
	vidbuf_out = vidbuf_in;
	vidbuf_in = (vidbuf_in + 1) % 2;
	show_video_bitmap(vidbufs[vidbuf_out]);
#endif
}
