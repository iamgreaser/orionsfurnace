/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "gfx/gfx.h"

#include "gfx/font.h"
#include "gfx/sprite.h"

#include "gfx/internal.h"

#include "core/version.h"

#include <SDL.h>

#include <cassert>
#include <string>

namespace gfx
{
	// 1x = 640x360
	// 2x = 1280x720
	// 3x = 1920x1080
	constexpr int INITIAL_SCREEN_SCALE = 2;
	int current_window_width = BASE_SCREEN_WIDTH*INITIAL_SCREEN_SCALE;
	int current_window_height = BASE_SCREEN_HEIGHT*INITIAL_SCREEN_SCALE;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *backbuf = NULL;

#if 0
	BITMAP *vidbufs[2] = {};
	int vidbuf_in = 0;
	int vidbuf_out = 1;
#endif
}

void gfx::init(void)
{
	// Create a window
	std::string window_title = (
		std::string("orion's furnace v")
		+ get_engine_version());
	window = SDL_CreateWindow(
		window_title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		current_window_width,
		current_window_height,
		(0
			//| SDL_WINDOW_RESIZABLE;
			));
	assert(window != NULL);

	// Create a renderer for the window
	renderer = SDL_CreateRenderer(
		window,
		-1,
		(0
			| SDL_RENDERER_PRESENTVSYNC
			| SDL_RENDERER_TARGETTEXTURE
			));
	assert(renderer != NULL);

	// Create our backbuffer
	backbuf = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_BGRA8888,
		SDL_TEXTUREACCESS_TARGET,
		BASE_SCREEN_WIDTH,
		BASE_SCREEN_HEIGHT);
	assert(backbuf != NULL);

	// Target the backbuffer by default
	int did_set_render_target = SDL_SetRenderTarget(
		renderer, backbuf);
	assert(did_set_render_target == 0);

	// Initialise SDL_ttf
	init_font();

	// Load some assets
	tile_gfx_floor.ensure_loaded();
	player_gfx.ensure_loaded();
}

void gfx::clear(int r, int g, int b)
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	SDL_RenderFillRect(renderer, NULL);
}

void gfx::clip_nothing(void)
{
	// Clear clipping rectangle
	int did_set_clip_rect = SDL_RenderSetClipRect(renderer, NULL);
	assert(did_set_clip_rect == 0);
}

void gfx::clip_rect(int px, int py, int pw, int ph)
{
	// Set clipping rectangle
	SDL_Rect rect = {};
	rect.x = px;
	rect.y = py;
	rect.w = pw;
	rect.h = ph;

	int did_set_clip_rect = SDL_RenderSetClipRect(renderer, &rect);
	assert(did_set_clip_rect == 0);
}

void gfx::draw_text(int px, int py, int r, int g, int b, const std::string text)
{
	// Draw text
	font_base.draw(px, py, r, g, b, text);
}

void gfx::flip(void)
{
	// Clear render target back to default
	int did_clear_render_target = SDL_SetRenderTarget(
		renderer, NULL);
	assert(did_clear_render_target == 0);

	// Blit
	int did_blit = SDL_RenderCopy(
		renderer, backbuf, NULL, NULL);
	assert(did_blit == 0);

	// Present
	SDL_RenderPresent(renderer);

	// Set render target again
	int did_set_render_target = SDL_SetRenderTarget(
		renderer, backbuf);
	assert(did_set_render_target == 0);
}
