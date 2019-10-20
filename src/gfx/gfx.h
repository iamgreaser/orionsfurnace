#ifndef _GFX_GFX_H
#define _GFX_GFX_H

#include <string>

namespace gfx
{
	constexpr int BASE_SCREEN_WIDTH = 640;
	constexpr int BASE_SCREEN_HEIGHT = 360;
	constexpr int CELL_W = 24;
	constexpr int CELL_H = 24;
	constexpr int CAM_X = 0;
	constexpr int CAM_Y = 0;
	constexpr int CAM_W_CELLS = 15;
	constexpr int CAM_H_CELLS = 15;
	constexpr int CAM_W = CELL_W*CAM_W_CELLS;
	constexpr int CAM_H = CELL_H*CAM_H_CELLS;

	constexpr int SIDEBAR_X = CAM_W;
	constexpr int SIDEBAR_Y = 0;
	constexpr int SIDEBAR_W = BASE_SCREEN_WIDTH-SIDEBAR_X;
	constexpr int SIDEBAR_H = BASE_SCREEN_HEIGHT;

	void init(void);

	void clear(int r, int g, int b);
	void clip_nothing(void);
	void clip_rect(int px, int py, int pw, int ph);
	void draw_text(int px, int py, int r, int g, int b, const std::string text);

	void flip(void);
}

using gfx::CELL_W;
using gfx::CELL_H;
using gfx::CAM_X;
using gfx::CAM_Y;
using gfx::CAM_W_CELLS;
using gfx::CAM_H_CELLS;
using gfx::CAM_W;
using gfx::CAM_H;
using gfx::SIDEBAR_X;
using gfx::SIDEBAR_Y;
using gfx::SIDEBAR_W;
using gfx::SIDEBAR_H;

#endif /* if !defined(_GFX_GFX_H) */
