#include "core/player.h"

#include "core/core.h"
#include "core/game.h"
#include "gfx/gfx.h"
#include "gfx/sprite.h"

#include <cassert>
#include <cstdint>
#include <iostream>
using std::istream;
using std::ostream;

Player::Player(Game *game, int cx, int cy, Direction dir)
	: m_game(game)
	, m_cx(cx)
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
			if (this->attempt_move_by(dx, dy)) {
				m_input_cooldown = 12;

			}
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
	this->calc_interp_pos(&px, &py);
	gfx::player_gfx.draw(px, py, m_dir, 0);
}

void Player::load_this(istream &ips)
{
	load(ips, m_cx);
	load(ips, m_cy);
	uint8_t ui_dir = 0;
	load(ips, ui_dir);
	assert(ui_dir < 4);
	m_dir = (Direction)ui_dir;
	load(ips, m_pos_interp_x0);
	load(ips, m_pos_interp_y0);
	load(ips, m_pos_interp_x1);
	load(ips, m_pos_interp_y1);
	load(ips, m_pos_interp_remain);
	load(ips, m_pos_interp_len);
	for (int i = 0; i < 4; i++) {
		load(ips, m_input_move[i]);
	}
	load(ips, m_input_cooldown);
}

void Player::save_this(ostream &ops)
{
	save(ops, m_cx);
	save(ops, m_cy);
	uint8_t ui_dir = (uint8_t)m_dir;
	save(ops, ui_dir);
	save(ops, m_pos_interp_x0);
	save(ops, m_pos_interp_y0);
	save(ops, m_pos_interp_x1);
	save(ops, m_pos_interp_y1);
	save(ops, m_pos_interp_remain);
	save(ops, m_pos_interp_len);
	for (int i = 0; i < 4; i++) {
		save(ops, m_input_move[i]);
	}
	save(ops, m_input_cooldown);
}

void Player::calc_interp_pos(int *px, int *py)
{
	// Calculate position
	*px = m_pos_interp_x1;
	*py = m_pos_interp_y1;

	if (m_pos_interp_remain > 0) {
		int dpx = (m_pos_interp_x1 - m_pos_interp_x0);
		int dpy = (m_pos_interp_y1 - m_pos_interp_y0);
		int t = m_pos_interp_remain;
		int l = m_pos_interp_len;
		*px -= (2*dpx*t+l)/(2*l);
		*py -= (2*dpy*t+l)/(2*l);
	}
}

bool Player::attempt_move_by(int dx, int dy)
{
	int cx = m_cx + dx;
	int cy = m_cy + dy;

	// Is there a player at the given cell?
	Player *other = m_game->get_player_at(cx, cy);
	if (other != NULL) {
		// Is it us?
		if (other == this) {
			// Don't push ourselves, that causes stack overflows.
			// I've crashed ZZT enough times to know to do this. --GM
			return false;
		}

		// Can we push it?
		if (!other->attempt_move_by(dx, dy)) {
			return false;
		}
	}

	// Face this direction.
	if (dy < 0) { m_dir = direction::NORTH; }
	if (dy > 0) { m_dir = direction::SOUTH; }
	if (dx < 0) { m_dir = direction::WEST; }
	if (dx > 0) { m_dir = direction::EAST; }

	// Move to the next cell.
	return this->attempt_move_to(cx, cy);
}

bool Player::attempt_move_to(int cx, int cy)
{
	// TEST: Is this in range of the map?
	if (cx < 0 || cy < 0 || cx >= 15 || cy >= 15) {
		return false;
	}

	// Is there a player at the given cell?
	Player *other = m_game->get_player_at(cx, cy);
	if (other != NULL) {
		return false;
	}

	// Move.
	m_cx = cx;
	m_cy = cy;

	// Interpolate.
	int new_px = m_pos_interp_x1;
	int new_py = m_pos_interp_y1;
	this->calc_interp_pos(&new_px, &new_py);
	m_pos_interp_x0 = new_px;
	m_pos_interp_y0 = new_py;
	m_pos_interp_x1 = m_cx*CELL_W;
	m_pos_interp_y1 = m_cy*CELL_H;
	m_pos_interp_remain = 12+4;
	m_pos_interp_len = m_pos_interp_remain;

	// Acknowledge.
	return true;
}
