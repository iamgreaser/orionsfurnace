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
	class GameFrame;
}
using game::Game;
using game::GameFrame;

class Player;
class PlayerInput;

#endif /* if !defined(_CORE_CORE_H) */
