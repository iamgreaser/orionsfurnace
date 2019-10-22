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

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::add_player(Player player)
{
	m_players.push_back(player);
}

void Game::player_set_all_inputs(int player_idx, PlayerInput player_input)
{
	assert(player_idx >= 0 && player_idx < (int)m_players.size());
	m_players[player_idx].set_all_inputs(player_input);
}

void Game::tick(const GameFrame &game_frame)
{
	assert(game_frame.get_player_count() == this->get_player_count());

	for (int i = 0; i < this->get_player_count(); i++) {
		this->player_set_all_inputs(i,
			game_frame.player_get_all_inputs(i));
	}

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

GameFrame::GameFrame(int player_count)
{
	for (int i = 0; i < player_count; i++) {
		m_player_inputs.push_back(PlayerInput());
	}
}
GameFrame::GameFrame(std::istream &ips)
{
	load(ips, *this);
}

void GameFrame::load_this(istream &ips)
{
	uint16_t player_count = 0;
	load(ips, player_count);
	m_player_inputs.clear();

	for (uint16_t i = 0; i < player_count; i++) {
		m_player_inputs.push_back(PlayerInput(ips));
	}
}

void GameFrame::save_this(ostream &ops)
{
	uint16_t player_count = m_player_inputs.size();
	save(ops, player_count);

	for (uint16_t i = 0; i < player_count; i++) {
		save(ops, m_player_inputs[i]);
	}
}
