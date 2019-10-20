#ifndef _CORE_PLAYER_H
#define _CORE_PLAYER_H

#include "core/core.h"

class Player
{
private:
	int m_x;
	int m_y;
	Direction m_dir;
public:
	Player(int cx, int cy, Direction dir);

	int get_x(void) { return m_x; }
	int get_y(void) { return m_y; }
	Direction get_dir(void) { return m_dir; }

	void move(int dx, int dy);

	void draw(void);
};

#endif /* if !defined(_CORE_PLAYER_H) */
