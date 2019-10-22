#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "core/core.h"
#include "core/game.h"
#include "net/net.h"

#include <iostream>
#include <fstream>
#include <vector>

namespace net
{
	class ServerClient : public Node
	{
	public:
		ServerClient(std::istream &ips, std::ostream &ops);
		~ServerClient(void);
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

