#ifndef _CORE_GAME_H
#define _CORE_GAME_H

#include "core/core.h"

#include "core/player.h"
#include "core/save.h"

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
	public:
		Game();
		~Game();

		int get_player_count(void) { return m_players.size(); }
		Player *get_player_ptr(int pidx) { return &m_players[pidx]; }

		void player_set_all_inputs(int player_idx,
			PlayerInput player_input);

		Player *get_player_at(int cx, int cy);
		void tick(void);
		void draw(void);

		void load_this(istream &ips);
		void save_this(ostream &ops);
	};
}

#endif /* if !defined(_CORE_GAME_H) */
