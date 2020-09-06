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

#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include "core/core.h"

#include "core/entity.h"
#include "core/save.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>

class PlayerInput final : public Saveable
{
private:
  bool m_input_move[4] = {};
public:
  PlayerInput(void);
  PlayerInput(std::istream &ips);

  bool get_input_move(Direction dir) {
    return m_input_move[dir];
  }
  void set_input_move(Direction dir, bool v) {
    m_input_move[dir] = v;
  }

  void load_this(std::istream &ips) override;
  void save_this(std::ostream &ops) const override;
};

class Player final : public Saveable
{
private:
  Game *m_game;
  // TODO: Put World in charge of this --GM
  Entity m_entity;

  PlayerInput m_input;

public:
  Player(Game *game, int cx, int cy, Direction dir);
  Player(Game *game, std::istream &ips);

  int get_x(void) { return m_entity.get_x(); }
  int get_y(void) { return m_entity.get_y(); }
  Entity *get_entity(void) { return &m_entity; }
  Direction get_dir(void) { return m_entity.get_dir(); }
  bool get_input_move(Direction dir) {
    return m_input.get_input_move(dir);
  }
  void set_input_move(Direction dir, bool v) {
    m_input.set_input_move(dir, v);
  }
  void set_all_inputs(PlayerInput player_input);

  void tick(void);

  void draw(void);

  void load_this(std::istream &ips) override;
  void save_this(std::ostream &ops) const override;
};

#endif /* if !defined(CORE_PLAYER_H) */
