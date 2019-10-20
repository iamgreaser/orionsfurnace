#include "core/player.h"

#include "core/core.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"

#include <cassert>

Player::Player(int cx, int cy, Direction dir)
	: m_cx(cx)
	, m_cy(cy)
	, m_dir(dir)
	, m_pos_interp_x0(cx*CELL_W)
	, m_pos_interp_y0(cy*CELL_H)
	, m_pos_interp_x1(cx*CELL_W)
	, m_pos_interp_y1(cy*CELL_H)
{
}

void Player::tick(void)
{
	// Calculate movement direction
	int dx = 0;
	int dy = 0;

	if (m_input_move[direction::NORTH]) { dy -= 1; }
	if (m_input_move[direction::SOUTH]) { dy += 1; }
	if (m_input_move[direction::WEST]) { dx -= 1; }
	if (m_input_move[direction::EAST]) { dx += 1; }

	assert (dx >= -1 && dx <= +1);
	assert (dy >= -1 && dy <= +1);

	// Calculate facing direction base on movement direction
	// Prioritise horizontal over vertical
	if (dy < 0) { m_dir = direction::NORTH; }
	if (dy > 0) { m_dir = direction::SOUTH; }
	if (dx < 0) { m_dir = direction::WEST; }
	if (dx > 0) { m_dir = direction::EAST; }

	// Apply input cooldown
	if (m_input_cooldown <= 0) {
		// If we're moving, then move.
		if (dx != 0 || dy != 0) {
			m_cx += dx;
			m_cy += dy;
			m_input_cooldown = 12;

			// TODO: Snap to the current point, not the final point
			m_pos_interp_x0 = m_pos_interp_x1;
			m_pos_interp_y0 = m_pos_interp_y1;
			m_pos_interp_x1 = m_cx*CELL_W;
			m_pos_interp_y1 = m_cy*CELL_H;
			m_pos_interp_remain = m_input_cooldown;
			m_pos_interp_len = m_pos_interp_remain;
		}
	}

	if (m_input_cooldown > 0) {
		m_input_cooldown -= 1;
	}

	// Apply pixel position interpolation
	// TODO: Make a timestamp and do the interp in draw()
	if (m_pos_interp_remain > 0) {
		m_pos_interp_remain -= 1;
		if (m_pos_interp_remain == 0) {
			// We're done with interpolation.
			// Snap to position.
			m_pos_interp_len = 0;
			m_pos_interp_x0 = m_pos_interp_x1;
			m_pos_interp_y0 = m_pos_interp_y1;
		}
	}
}

void Player::draw(void)
{
	// Calculate position
	int px = m_pos_interp_x1;
	int py = m_pos_interp_y1;

	if (m_pos_interp_remain > 0) {
		int dpx = (m_pos_interp_x1 - m_pos_interp_x0);
		int dpy = (m_pos_interp_y1 - m_pos_interp_y0);
		int t = m_pos_interp_remain;
		int l = m_pos_interp_len;
		px -= (2*dpx*t+l)/(2*l);
		py -= (2*dpy*t+l)/(2*l);
	}

	gfx::player_gfx.draw(px, py, m_dir, 0);
}
