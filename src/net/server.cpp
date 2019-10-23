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
	this->update_packets();
}

void ServerClient::handle_input_packet(int packet_id, std::istream &packet_ss)
{
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

////////////////////////////////////////////////////////////////////////////

Server::Server(void)
{
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
	int client_index = m_clients.size();
	int player_index = m_game.get_player_count();

	// TODO: Add spawn points
	// FIXME: This COULD spawn one player atop another, or atop a wall or something!
	// (but at least it terminates)
	int cx = (int)m_game.random().next_int((uint32_t)m_game.get_width());
	int cy = (int)m_game.random().next_int((uint32_t)m_game.get_height());
	for (int i = 0; i < 100; i++) {
		if (m_game.can_step_into(cx, cy, true)) {
			// Our position is good!
			break;
		}
		cx = (int)m_game.random().next_int((uint32_t)m_game.get_width());
		cy = (int)m_game.random().next_int((uint32_t)m_game.get_height());
	}

	// Add the player
	m_game.add_player(Player(&m_game, cx, cy, direction::SOUTH));

	// Add the player to everyone else
	// DO THIS BEFORE ADDING THE NEW CLIENT.
	// TODO: Cut down on redundancy here and use the same Player object
	net::AddPlayerPacket add_player_packet(PlayerAdd(
		player_index, Player(&m_game, cx, cy, direction::SOUTH)));
	this->broadcast_packet(add_player_packet);

	// Add a new client
	m_clients.push_back(ServerClient(this, player_index, ips, ops));

	// Send a game snapshot to the new client
	// TODO: Wait for the "Hello" packet and then send the player's index if the Hello is OK
	net::GameSnapshotPacket game_snapshot_packet(m_game);
	m_clients[client_index].send_packet(game_snapshot_packet);
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
