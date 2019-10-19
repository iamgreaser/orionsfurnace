#include "core/player.h"

#include "core/core.h"
#include "gfx/gfx.h"

#include <allegro.h>

Player::Player(int x, int y, Direction dir)
	: m_x(x)
	, m_y(y)
	, m_dir(dir)
{
}

void Player::move(int dx, int dy)
{
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
