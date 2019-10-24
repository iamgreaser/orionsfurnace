#include "net/client.h"

#include "core/core.h"
#include "core/game.h"
#include "core/version.h"
#include "net/net.h"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

using net::Client;
using net::ClientHello;
using net::GameSnapshotPacket;
using net::GameFramePacket;

ClientHello::ClientHello(std::string nickname)
	: m_version(get_engine_version())
	, m_nickname(nickname)
{
}

ClientHello::ClientHello(std::istream &ips)
{
	load(ips, *this);
}

bool ClientHello::is_current_version(void)
{
	return (m_version == get_engine_version());
}

void ClientHello::load_this(std::istream &ips)
{
	load(ips, m_version);
	load(ips, m_nickname);
}

void ClientHello::save_this(std::ostream &ops)
{
	save(ops, m_version);
	save(ops, m_nickname);
}


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

int Client::get_player_idx(void)
{
	return m_player_idx;
}

PlayerInput Client::get_player_input(void)
{
	return m_player_input;
}

void Client::set_all_inputs(PlayerInput player_input)
{
	m_player_input = player_input;
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

	switch (m_status)
	{
		case client_status::SENDING_HELLO:
			// Send a Hello packet.
			// TODO!

			// Now we move to the next awaiting state.
			m_status = client_status::LOADING_GAME;
			break;

		case client_status::LOADING_GAME:
			// All we can do here really is wait.
			// Until we've got everything we need that is...
			if (this->m_game == NULL) { break; }
			if (this->m_player_idx < 0) { break; }

			// ALRIGHT WE'RE GOOD NOW
			this->m_status = client_status::PLAYING_GAME;
			break;

		case client_status::PLAYING_GAME:
			// Now we can spam inputs everywhere it'll be great
			this->tick_input_send();
			break;

		case client_status::DISCONNECTED:
			// Welp, time to show a message telling people to quit.
			// TODO: implement that in the gameloop.
			break;

	}
}

void Client::tick_input_send(void)
{
	if (m_ready_to_send_input) {
		net::ProvideInputPacket provide_input_packet(m_player_input);
		this->send_packet(provide_input_packet);
		m_ready_to_send_input = false;
	}
}

void Client::handle_input_packet(int packet_id, std::istream &packet_ss)
{
	// Select by packet ID.
	switch (packet_id)
	{
		case packets::THIS_IS_YOU: {
			// Load player index.
			std::cout << "Load player index" << std::endl;
			int16_t player_idx = -1;
			load(packet_ss, player_idx);
			m_player_idx = player_idx;
		} break;

		case packets::GAME_SNAPSHOT: {
			// Load game.
			std::cout << "Load game" << std::endl;
			this->load_game(packet_ss);
			assert(m_game != NULL);
		} break;

		case packets::GAME_FRAME: {
			// Apply frame inputs.
			std::cout << "Apply frame inputs" << std::endl;
			GameFrame game_frame(packet_ss);
			if (m_game != NULL) {
				m_game->tick(game_frame);
			}
			m_ready_to_send_input = true;
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
