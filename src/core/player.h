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
	Player(int x, int y, Direction dir);

	void move(int dx, int dy);

	void draw(void);
};

#endif /* if !defined(_CORE_PLAYER_H) */
