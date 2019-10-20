#ifndef _CORE_PLAYER_H
#define _CORE_PLAYER_H

#include "core/core.h"

#include <cassert>

class Player
{
private:
	int m_x;
	int m_y;
	Direction m_dir;
	bool m_input_move[4] = {};

public:
	Player(int cx, int cy, Direction dir);

	int get_x(void) { return m_x; }
	int get_y(void) { return m_y; }
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
};

#endif /* if !defined(_CORE_PLAYER_H) */
