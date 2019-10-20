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
	uint8_t player_count = 0;
	load(ips, player_count);
}

void Game::save_this(ostream &ops)
{
	uint8_t player_count = m_players.size();
	save(ops, player_count);
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
