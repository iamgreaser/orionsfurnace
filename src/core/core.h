/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

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
class PlayerAdd;

#endif /* if !defined(_CORE_CORE_H) */
