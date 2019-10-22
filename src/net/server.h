#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "core/core.h"
#include "core/game.h"

#include <vector>

namespace net
{
	class ServerClient : public Node
	{
	public:
		ServerClient(std::istream &ips, std::ostream &ops);
		~ServerClient(void);
		void update(void);
	};

	class Server
	{
	private:
		std::vector<ServerClient> m_clients;
		Game m_game;
	public:
		Server(void);
		~Server(void);

		Game &game(void);

		void add_client(std::istream &ips, std::ostream &ops);

		void game_tick(GameFrame game_frame);
		void update(void);
	};
}

#endif /* if !defined(_NET_SERVER_H) */

