#ifndef _CORE_GAME_H
#define _CORE_GAME_H

#include "core/core.h"

#include "core/player.h"

#include <vector>

namespace game
{
	using std::vector;

	class Game
	{
	private:
		vector<Player> m_players;
	public:
		Game();
		~Game();

		int get_player_count(void) { return m_players.size(); }
		Player *get_player_ptr(int pidx) { return &m_players[pidx]; }

		void set_player_input_move(
			int player_idx, Direction dir, bool v);

		void tick(void);
		void draw(void);
	};
}

#endif /* if !defined(_CORE_GAME_H) */

