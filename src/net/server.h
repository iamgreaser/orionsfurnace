#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "net/net.h"

#include <iostream>
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
		Server *m_server = NULL;
		std::string m_input_buf;
		GameFrame m_game_frame;
		int m_player_index;
		PlayerInput m_player_input;
	public:
		ServerClient(Server *server, int player_index, std::istream &ips, std::ostream &ops);
		~ServerClient(void);
		PlayerInput get_player_input(void) {
			return m_player_input;
		}
		void update(void);

		void send_packet(Packet &packet);
	};

	class Server
	{
	private:
		std::ofstream *m_demo_fp = NULL;
		std::vector<ServerClient> m_clients;
		Game m_game;
	public:
		Server(void);
		~Server(void);

		Game &game(void);

		void add_client(std::istream &ips, std::ostream &ops);

		void broadcast_packet(net::Packet &packet);

		void quicksave(void);
		void quickload(void);

		void game_tick(GameFrame game_frame);
		void update(void);
	};
}

#endif /* if !defined(_NET_SERVER_H) */

