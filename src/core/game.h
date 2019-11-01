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

#ifndef CORE_GAME_H
#define CORE_GAME_H

#include "core/core.h"

#include "core/player.h"
#include "core/random.h"
#include "core/save.h"
#include "core/world.h"
#include "net/net.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <map>
#include <stdexcept>

namespace game
{
  using std::istream;
  using std::ostream;
  using std::map;

  class Game final : public Saveable
  {
  private:
    map<uint16_t, Player> m_players;
    std::shared_ptr<World> m_world = nullptr;
    Random m_random;
  public:
    Game(void);
    Game(std::istream &ips);
    ~Game(void) override;

    std::map<uint16_t, Player>::iterator player_begin(void) {
      return m_players.begin();
    }
    std::map<uint16_t, Player>::iterator player_end(void) {
      return m_players.end();
    }
    int get_player_count(void) const {
      size_t count = m_players.size();
      assert(count < 0xFFFF);
      return static_cast<int>(count);
    }
    Player *get_player_ptr(int player_idx) {
      assert(player_idx >= 0 && player_idx < 0xFFFF);
      uint16_t key = static_cast<uint16_t>(player_idx);

      // Apparently, Windows can't handle C++11 exceptions.
      // Yeah, your guess is good as mine.
      //
      // If you use Windows and C++11 exceptions work for you,
      // would you like to maintain the Windows stuff? :) --GM

      if (m_players.find(key) == m_players.end()) {
        return nullptr;
      } else {
        return &m_players.at(key);
      }
    }
    int get_width(void) const { return static_cast<int>(m_world.get()->get_width()); }
    int get_height(void) const { return static_cast<int>(m_world.get()->get_height()); }
    Random &random(void) { return m_random; }

    void add_player(int player_idx, Player player);
    void remove_player(int player_idx);
    void spawn_player(int player_idx);
    void player_set_all_inputs(int player_idx,
      PlayerInput player_input);

    bool can_step_into(int cx, int cy, bool players_are_blocking);
    Player *get_player_at(int cx, int cy);

    void tick(const GameFrame &game_frame);
    void draw(void);

    void load_this(istream &ips) override;
    void save_this(ostream &ops) const override;
  };

  class GameFrame : public Saveable
  {
    friend Game;
  private:
    map<uint16_t, PlayerInput> m_player_inputs;
  public:
    GameFrame(void);
    GameFrame(std::istream &ips);

    int get_player_count(void) const {
      size_t count = m_player_inputs.size();
      assert(count < 0xFFFF);
      return static_cast<int>(count);
    }

#if 0
    // FIXME: reinstate this once I've made some sense of move constructors --GM
    void player_set_all_inputs(int player_idx,
      PlayerInput &&player_input)
    {
      assert(player_idx >= 0 && player_idx < 0xFFFF);
      uint16_t key = static_cast<uint16_t>(player_idx);
      try {
        m_player_inputs.at(key) = std::move(player_input);
      } catch (std::out_of_range &e) {
        m_player_inputs.emplace(key, std::move(player_input));
      }
    }
#endif

    void player_set_all_inputs(int player_idx,
      const PlayerInput &player_input)
    {
      assert(player_idx >= 0 && player_idx < 0xFFFF);
      uint16_t key = static_cast<uint16_t>(player_idx);

      if (m_player_inputs.find(key) == m_player_inputs.end()) {
        m_player_inputs.emplace(key, player_input);
      } else {
        m_player_inputs.at(key) = player_input;
      }
    }

    const PlayerInput &player_get_all_inputs(int player_idx) const
    {
      assert(player_idx >= 0 && player_idx < 0xFFFF);
      return m_player_inputs.at(static_cast<uint16_t>(player_idx));
    }

    void load_this(istream &ips) override;
    void save_this(ostream &ops) const override;
  };
}

#endif /* if !defined(CORE_GAME_H) */
