#include "core/game.h"

#include "core/core.h"
#include "core/helpers.h"
#include "core/save.h"

#include <cstdint>
#include <iostream>
using std::istream;
using std::ostream;
#include <vector>
using std::vector;

namespace game
{
}

Game::Game()
{
	m_players.push_back(Player(this,  4,  8, direction::SOUTH));
	m_players.push_back(Player(this, 10,  6, direction::SOUTH));
}

Game::~Game()
{
}

void Game::set_player_input_move(int player_idx, Direction dir, bool v)
{
	assert(player_idx >= 0 && player_idx < (int)m_players.size());
	m_players[player_idx].set_input_move(dir, v);
}

void Game::tick(void)
{
	for (Player &p : m_players) {
		p.tick();
	}
}

void Game::draw(void)
{
	for (Player &p : m_players) {
		p.draw();
	}
}

void Game::load_this(istream &ips)
{
	uint16_t player_count = 0;
	load(ips, player_count);
	m_players.clear();

	for (uint16_t i = 0; i < player_count; i++) {
		m_players.push_back(Player(this, ips));
	}
}

void Game::save_this(ostream &ops)
{
	uint16_t player_count = m_players.size();
	save(ops, player_count);

	for (uint16_t i = 0; i < player_count; i++) {
		save(ops, m_players[i]);
	}
}

Player *Game::get_player_at(int cx, int cy)
{
	for (Player &p : m_players) {
		if (p.get_x() == cx && p.get_y() == cy) {
			return &p;
		}
	}

	return NULL;
}
