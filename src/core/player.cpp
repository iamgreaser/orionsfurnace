#include "core/player.h"

#include "core/core.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"

#include <cassert>

Player::Player(int cx, int cy, Direction dir)
	: m_cx(cx)
	, m_cy(cy)
	, m_px(cx*CELL_W)
	, m_py(cy*CELL_H)
	, m_dir(dir)
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
			m_px += dx*CELL_W;
			m_py += dy*CELL_H;
			m_input_cooldown = 5;
		}
	}

	if (m_input_cooldown > 0) {
		m_input_cooldown -= 1;
	}
}

void Player::draw(void)
{
	gfx::player_gfx.draw(m_px, m_py, m_dir, 0);
}
