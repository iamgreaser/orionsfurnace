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
			// TODO: Use these!
			SENDING_HELLO = 0,
			AWAITING_YOUR_PLAYER,
			AWAITING_GAME,
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
	public:
		Client(std::istream &ips, std::ostream &ops);
		~Client(void);

		Game *game(void);
		void load_game(std::istream &ips);

		void send_packet(Packet &packet);

		void update(void);
		void handle_input_packet(int packet_id, std::istream &packet_ss);
	};
}

#endif /* if !defined(_NET_CLIENT_H) */
