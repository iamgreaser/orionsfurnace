#ifndef _CORE_CORE_H
#define _CORE_CORE_H

namespace direction
{
	enum Direction {
		NORTH = 0,
		SOUTH = 1,
		WEST  = 2,
		EAST  = 3,
	};
}
using direction::Direction;

namespace game
{
	class Game;
}
using game::Game;

class Player;
class PlayerInput;

#endif /* if !defined(_CORE_CORE_H) */
