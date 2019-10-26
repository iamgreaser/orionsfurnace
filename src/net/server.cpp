/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "net/server.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "core/version.h"
#include "net/net.h"

using net::Server;
using net::ServerClient;

ServerClient::ServerClient(Server *server, int player_index, net::PipeEnd *pipe_end)
	: net::Node::Node(pipe_end)
	, m_server(server)
	, m_player_index(player_index)
{
	std::cout << "New ServerClient! " << ((intptr_t)this) << std::endl;
}

ServerClient::~ServerClient(void)
{
	std::cout << "Old ServerClient! " << ((intptr_t)this) << std::endl;
	if (m_pipe_end != NULL) {
		delete m_pipe_end;
	}
	std::cout << "Old ServerClient DONE" << std::endl;
}

void ServerClient::update(void)
{
	m_pipe_end->pump_recv();

	this->update_packets();

	m_pipe_end->pump_send();
}

void ServerClient::handle_input_packet(int packet_id, std::istream &packet_ss)
{
	// Ignore if disconnected.
	if (m_disconnected) {
		return;
	}

	// Select by packet ID.
	switch (packet_id)
	{
		case packets::HELLO: {
			// Say hello!
			std::cout << "Hello from client" << std::endl;

			ClientHello hello(packet_ss);

			// Is this the correct version?
			if (!hello.is_current_version()) {
				// No - disconnect the client
				std::cout << "Client has the wrong version!" << std::endl;

				net::DisconnectPacket disconnect_packet(
					std::string("Version mismatch!\n")
					+ std::string("Please use v")
					+ get_engine_version()
				);
				this->send_packet(disconnect_packet);
				m_disconnected = true;
				break;
			}

			Game &game = m_server->game();
			m_player_index = game.get_player_count();

			// TODO: Add spawn points
			// FIXME: This COULD spawn one player atop another, or atop a wall or something!
			// (but at least it terminates)
			int cx = (int)game.random().next_int((uint32_t)game.get_width());
			int cy = (int)game.random().next_int((uint32_t)game.get_height());
			for (int i = 0; i < 100; i++) {
				if (game.can_step_into(cx, cy, true)) {
					// Our position is good!
					break;
				}
				cx = (int)game.random().next_int((uint32_t)game.get_width());
				cy = (int)game.random().next_int((uint32_t)game.get_height());
			}

			// Add the player
			game.add_player(Player(&game, cx, cy, direction::SOUTH));

			// Add the player to everyone else
			// TODO: Cut down on redundancy here and use the same Player object
			net::AddPlayerPacket add_player_packet(PlayerAdd(
				m_player_index, Player(&game, cx, cy, direction::SOUTH)));
			m_server->broadcast_packet(add_player_packet);

			// Send a game snapshot to the client
			// Also send a "This Is You" packet
			net::ThisIsYouPacket this_is_you_packet(m_player_index);
			net::GameSnapshotPacket game_snapshot_packet(game);
			this->send_packet(this_is_you_packet);
			this->send_packet(game_snapshot_packet);
		} break;

		case packets::PROVIDE_INPUT: {
			// Set frame input.
			std::cout << "Provide input for player " << m_player_index << std::endl;
			assert(m_server != NULL);
			PlayerInput player_input(packet_ss);
			m_player_input = player_input;
		} break;

		default: {
			std::cerr << "ERR: Unhandled packet ID " << std::hex << packet_id << " on server recv" << std::dec << std::endl;
			abort();
		} break;
	}
}

////////////////////////////////////////////////////////////////////////////

Server::Server(int port)
	: m_tcp_server(port)
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

void Server::add_client(net::PipeEnd *pipe_end)
{
	//int client_index = m_clients.size();
	// Add a new client
	m_clients.push_back(std::shared_ptr<ServerClient>(
		new ServerClient(this, -1, pipe_end)));
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
	for (std::shared_ptr<ServerClient> sc : m_clients) {
		sc.get()->send_packet(packet);
	}
}

void Server::broadcast_packet_ignoring_client(net::Packet &packet, ServerClient *ignore_sc)
{
	// Start recording demo if we haven't yet
	if (m_demo_fp == NULL) {
		m_demo_fp = new std::ofstream("test.demo");
		net::GameSnapshotPacket game_snapshot_packet(m_game);
		save(*m_demo_fp, game_snapshot_packet);
	}

	save(*m_demo_fp, packet);
	for (std::shared_ptr<ServerClient> sc : m_clients) {
		if (sc.get() != ignore_sc) {
			sc.get()->send_packet(packet);
		}
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
	// Add a client if one is trying to connect
	net::TCPPipeEnd *pipe_end = m_tcp_server.accept_if_available();
	if (pipe_end != NULL) {
		std::cout << "Accepting new client" << std::endl;
		this->add_client(pipe_end);
	}

	// Form a frame
	GameFrame game_frame(m_game.get_player_count());

	for (int i = 0; i < m_game.get_player_count(); i++) {
		game_frame.player_set_all_inputs(
			i, m_clients[i]->get_player_input());
	}

	this->game_tick(game_frame);
	for (std::shared_ptr<ServerClient> sc : m_clients) {
		sc.get()->update();
	}
}
