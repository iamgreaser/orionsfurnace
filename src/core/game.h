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
#include "net/net.h"

#include <cstdint>
#include <iostream>
#include <vector>

namespace game
{
	using std::istream;
	using std::ostream;
	using std::vector;

	class Game : public Saveable
	{
	private:
		vector<Player> m_players;
		Random m_random;
	public:
		Game(void);
		Game(std::istream &ips);
		~Game(void);

		int get_player_count(void) { return m_players.size(); }
		Player *get_player_ptr(int pidx) { return &m_players[pidx]; }
		int get_width(void) const { return 15; }
		int get_height(void) const { return 15; }
		Random &random(void) { return m_random; }

		void add_player(Player player);
		void player_set_all_inputs(int player_idx,
			PlayerInput player_input);

		bool can_step_into(int cx, int cy, bool players_are_blocking);
		Player *get_player_at(int cx, int cy);

		void tick(const GameFrame &game_frame);
		void draw(void);

		void load_this(istream &ips);
		void save_this(ostream &ops);
	};

	class GameFrame : public Saveable
	{
	private:
		vector<PlayerInput> m_player_inputs;
	public:
		GameFrame(int player_count = 0);
		GameFrame(std::istream &ips);

		int get_player_count(void) const { return m_player_inputs.size(); }

		void player_set_all_inputs(int player_idx,
			PlayerInput player_input)
		{
			m_player_inputs[player_idx] = player_input;
		}

		PlayerInput player_get_all_inputs(int player_idx) const
		{
			return m_player_inputs[player_idx];
		}

		void load_this(istream &ips);
		void save_this(ostream &ops);
	};
}

#endif /* if !defined(CORE_GAME_H) */
