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

#ifndef CORE_CORE_H
#define CORE_CORE_H

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

namespace cell_type
{
  enum CellType {
    SPACE = 0,
    FLOOR,
    WALL,
  };
}

using cell_type::CellType;
namespace game
{
  class Game;
  class GameFrame;
}
using game::Game;
using game::GameFrame;

namespace world
{
  class World;
}
using world::World;

namespace entity
{
  class Entity;
}
using entity::Entity;

class Player;
class PlayerInput;
class PlayerAdd;

#endif /* if !defined(CORE_CORE_H) */
