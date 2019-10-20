#include "core/game.h"

#include "core/core.h"

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
