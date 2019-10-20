#ifndef _CORE_PLAYER_H
#define _CORE_PLAYER_H

#include "core/core.h"

#include <cassert>

class Player
{
private:
	Game *m_game;
	int m_cx;
	int m_cy;
	Direction m_dir;

	int m_pos_interp_x0;
	int m_pos_interp_y0;
	int m_pos_interp_x1;
	int m_pos_interp_y1;
	int m_pos_interp_remain = 0;
	int m_pos_interp_len = 0;

	bool m_input_move[4] = {};
	int m_input_cooldown = 0;

public:
	Player(Game *game, int cx, int cy, Direction dir);

	int get_x(void) { return m_cx; }
	int get_y(void) { return m_cy; }
	Direction get_dir(void) { return m_dir; }
	bool get_input_move(Direction dir) {
		assert(dir >= 0 && dir < 4);
		return m_input_move[dir];
	}
	void set_input_move(Direction dir, bool v) {
		assert(dir >= 0 && dir < 4);
		m_input_move[dir] = v;
	}

	void tick(void);

	void draw(void);

protected:
	bool attempt_move_by(int dx, int dy);
	bool attempt_move_to(int cx, int cy);
	void calc_interp_pos(int *px, int *py);
};

#endif /* if !defined(_CORE_PLAYER_H) */
