#ifndef _NET_CLIENT_H
#define _NET_CLIENT_H

#include "core/core.h"
#include "core/game.h"

#include <iostream>
#include <sstream>
#include <string>

namespace net
{
	namespace client_status
	{
		enum ClientStatus
		{
			WAITING_FOR_GAME = 0,
			PLAYING_GAME,
			DISCONNECTED,
		};
	}
	using client_status::ClientStatus;

	class Client : public Node
	{
	private:
		ClientStatus m_status;
		Game *m_game = NULL;
		std::string m_input_buf;
	public:
		Client(std::istream &ips, std::ostream &ops);
		~Client(void);

		Game *game(void);
		void load_game(std::istream &ips);

		void update(void);
	};
}

#endif /* if !defined(_NET_CLIENT_H) */
