#include "net/server.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "net/net.h"

using net::Server;
using net::ServerClient;

ServerClient::ServerClient(Server *server, int player_index, std::istream &ips, std::ostream &ops)
	: net::Node::Node(ips, ops)
	, m_server(server)
	, m_player_index(player_index)
{
}

ServerClient::~ServerClient(void)
{
}

void ServerClient::send_packet(net::Packet &packet)
{
	save(*m_ops, packet);
}

void ServerClient::update(void)
{
	// Fetch some input
	{
		char buf[8192];
		std::streamsize sz = m_ips->readsome(buf, 8192);
		m_input_buf += std::string(buf, sz);
	}

	// Extract packets
	for (;;) {
		size_t buffer_size = m_input_buf.size();
		std::cout << "ServerClient input:" << buffer_size << std::endl;

		// Do we have enough stuff to read to make any sense of this?
		if (buffer_size < 5) {
			// No - wait.
			std::cout << "Done " << std::endl;
			break;
		}

		// Extract the bare minimum info needed
		uint32_t sub_size = 0;
		sub_size += ((uint32_t)m_input_buf[0]);
		sub_size += ((uint32_t)m_input_buf[1])<<8;
		sub_size += ((uint32_t)m_input_buf[2])<<16;
		sub_size += ((uint32_t)m_input_buf[3])<<24;
		assert(sub_size >= 1);
		uint8_t packet_id = (uint8_t)m_input_buf[4];
		size_t full_packet_size = 4+1+sub_size;

		// Will it fit?
		if (buffer_size < full_packet_size) {
			// No - leave it in the buffer.
			std::cout << "Overshoot " << std::hex << sub_size << std::dec << std::endl;
			break;
		}

		// Get packet data.
		std::cout << "Client packet:" << full_packet_size << std::endl;
		std::string packet_s(m_input_buf, 4+1, full_packet_size-(4+1));
		m_input_buf.erase(m_input_buf.begin(), m_input_buf.begin()+full_packet_size);
		assert(m_input_buf.size() != buffer_size);
		std::stringstream packet_ss(packet_s);

		// Select by packet ID.
		switch (packet_id)
		{
			case packets::PROVIDE_INPUT: {
				// Set frame input.
				std::cout << "Provide input for player " << m_player_index << std::endl;
				assert(m_server != NULL);
				PlayerInput player_input(packet_ss);
				m_player_input = player_input;
			} break;


			default: {
				std::cerr << "ERR: Unhandled packet ID " << std::hex << packet_id << " on client recv" << std::dec << std::endl;
				abort();
			} break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////

Server::Server(void)
{
	m_game.add_player(Player(&m_game,  4,  8, direction::SOUTH));
	m_game.add_player(Player(&m_game, 10,  6, direction::SOUTH));
}

Server::~Server(void)
{
	if (m_demo_fp != NULL) {
		m_demo_fp->close();
		delete m_demo_fp;
		m_demo_fp = NULL;
	}
}

Game &Server::game(void)
{
	return m_game;
}

void Server::add_client(std::istream &ips, std::ostream &ops)
{
	int player_index = m_clients.size();

	m_clients.push_back(ServerClient(this, player_index, ips, ops));
	net::GameSnapshotPacket game_snapshot_packet(m_game);

	// NOTE: Comment the following line out if you wish to test client pre-game-snapshot state.
	// TODO: Wait for the "Hello" packet and then send the player's index if the Hello is OK
	m_clients[player_index].send_packet(game_snapshot_packet);
}

void Server::broadcast_packet(net::Packet &packet)
{
	// Start recording demo if we haven't yet
	if (m_demo_fp == NULL) {
		m_demo_fp = new std::ofstream("test.demo");
		net::GameSnapshotPacket game_snapshot_packet(m_game);
		save(*m_demo_fp, game_snapshot_packet);
	}

	save(*m_demo_fp, packet);
	for (ServerClient &sc : m_clients) {
		sc.send_packet(packet);
	}
}

void Server::game_tick(GameFrame game_frame)
{
	// Tick game
	m_game.tick(game_frame);

	// Add input to demo
	net::GameFramePacket game_frame_packet(game_frame);
	this->broadcast_packet(game_frame_packet);
}

void Server::quicksave(void)
{
	std::ofstream fp("quick.save");
	save(fp, m_game);
	fp.close();
}

void Server::quickload(void)
{
	std::ifstream fp("quick.save");
	load(fp, m_game);
	fp.close();

	// Broadcast to everyone
	net::GameSnapshotPacket game_snapshot_packet(m_game);
	this->broadcast_packet(game_snapshot_packet);
}

void Server::update(void)
{
	GameFrame game_frame(m_game.get_player_count());

	for (int i = 0; i < m_game.get_player_count(); i++) {
		game_frame.player_set_all_inputs(
			i, m_clients[i].get_player_input());
	}

	this->game_tick(game_frame);
	for (ServerClient &sc : m_clients) {
		sc.update();
	}
}
