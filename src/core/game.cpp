#include "core/game.h"

#include "core/core.h"
#include "core/helpers.h"

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

void Game::load(istream &ips)
{
	uint8_t player_count = 0;
	get_u8(ips, player_count);
}

void Game::save(ostream &ops)
{
	put_u8(ops, m_players.size());
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
