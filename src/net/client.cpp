#include "net/client.h"

#include "core/core.h"
#include "core/game.h"
#include "net/net.h"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

using net::Client;
using net::GameSnapshotPacket;
using net::GameFramePacket;

Client::Client(std::istream &ips, std::ostream &ops)
	: net::Node::Node(ips, ops)
{
}

Client::~Client(void)
{
	if (m_game != NULL) {
		delete m_game;
		m_game = NULL;
	}
}

Game *Client::game(void)
{
	return m_game;
}

void Client::load_game(std::istream &ips)
{
	if (m_game != NULL) {
		delete m_game;
		m_game = NULL;
	}

	m_game = new Game(ips);
}

void Client::send_packet(net::Packet &packet)
{
	save(*m_ops, packet);
}

void Client::update(void)
{
	this->update_packets();
}

void Client::handle_input_packet(int packet_id, std::istream &packet_ss)
{
	// Select by packet ID.
	switch (packet_id)
	{
		case packets::GAME_SNAPSHOT: {
			// Load game.
			std::cout << "Load game" << std::endl;
			this->load_game(packet_ss);
		} break;

		case packets::GAME_FRAME: {
			// Apply frame inputs.
			std::cout << "Apply frame inputs" << std::endl;
			GameFrame game_frame(packet_ss);
			if (m_game != NULL) {
				m_game->tick(game_frame);
			}
		} break;

		case packets::ADD_PLAYER: {
			// Add the new player
			std::cout << "Add new player" << std::endl;
			PlayerAdd player_add(packet_ss);
			if (m_game != NULL) {
				assert(player_add.get_player_idx() == m_game->get_player_count());
				m_game->add_player(player_add.get_player());
			}
		} break;

		default: {
			std::cerr << "ERR: Unhandled packet ID " << std::hex << packet_id << " on client recv" << std::dec << std::endl;
			abort();
		} break;
	}
}
