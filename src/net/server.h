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

#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "net/net.h"
#include "net/tcp.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

namespace net
{
	namespace server_client_status
	{
		enum ServerClientStatus
		{
			// TODO: Use these!
			AWAITING_HELLO = 0,
			SENDING_YOUR_PLAYER,
			SENDING_GAME,
			PLAYING_GAME,
			DISCONNECTED,
		};
	}
	using server_client_status::ServerClientStatus;

	class ServerClient : public Node
	{
	private:
		ServerClientStatus m_status;
		Server *m_server = nullptr;
		GameFrame m_game_frame;
		int m_player_index;
		PlayerInput m_player_input;
		bool m_disconnected = false;
	public:
		ServerClient(Server *server, int player_index, net::PipeEnd *pipe_end);
		~ServerClient(void);
		PlayerInput get_player_input(void) {
			return m_player_input;
		}
		void update(void) override;
		void handle_input_packet(int packet_id, std::istream &packet_ss) override;
	};

	class Server
	{
	private:
		std::ofstream *m_demo_fp = nullptr;
		std::vector<std::shared_ptr<ServerClient>> m_clients;
		net::TCPServer m_tcp_server;
		Game m_game;
	public:
		Server(int port);
		~Server(void);

		Game &game(void) {
			return m_game;
		}

		void add_client(net::PipeEnd *pipe_end);

		void broadcast_packet(net::Packet &packet);
		void broadcast_packet_ignoring_client(net::Packet &packet, ServerClient *ignore_sc);

		void quicksave(void);
		void quickload(void);

		void game_tick(GameFrame game_frame);
		void update(void);
	};
}

#endif /* if !defined(NET_SERVER_H) */
