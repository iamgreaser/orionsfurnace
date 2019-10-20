#include "core/player.h"

#include "core/core.h"
#include "gfx/gfx.h"

#include <allegro.h>

#include <cassert>

Player::Player(int cx, int cy, Direction dir)
	: m_x(cx*24)
	, m_y(cy*24)
	, m_dir(dir)
{
}

void Player::tick(void)
{
	int dx = 0;
	int dy = 0;

	if (m_input_move[direction::NORTH]) { dy -= 1; }
	if (m_input_move[direction::SOUTH]) { dy += 1; }
	if (m_input_move[direction::WEST]) { dx -= 1; }
	if (m_input_move[direction::EAST]) { dx += 1; }

	assert (dx >= -1 && dx <= +1);
	assert (dy >= -1 && dy <= +1);

	if (dy < 0) { m_dir = direction::NORTH; }
	if (dy > 0) { m_dir = direction::SOUTH; }
	if (dx < 0) { m_dir = direction::WEST; }
	if (dx > 0) { m_dir = direction::EAST; }

	m_x += dx;
	m_y += dy;
}

void Player::draw(void)
{
	draw_sprite(backbuf,
		gfx::player_gfx[m_dir],
		m_x, m_y);
}
