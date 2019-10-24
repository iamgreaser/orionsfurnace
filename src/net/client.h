#ifndef _NET_CLIENT_H
#define _NET_CLIENT_H

#include "core/core.h"
#include "core/game.h"
#include "core/save.h"

#include <iostream>
#include <sstream>
#include <string>

namespace net
{
	namespace client_status
	{
		enum ClientStatus
		{
			SENDING_HELLO = 0,
			LOADING_GAME,
			PLAYING_GAME,
			DISCONNECTED,
		};
	}
	using client_status::ClientStatus;

	class Client : public Node
	{
	private:
		ClientStatus m_status;
		PlayerInput m_player_input;
		int m_player_idx = -1;
		Game *m_game = NULL;

		// Throttle input sending to once a frame
		bool m_ready_to_send_input = false;
	public:
		Client(std::istream &ips, std::ostream &ops);
		~Client(void);

		Game *game(void);
		int get_player_idx(void);
		PlayerInput get_player_input(void);
		void set_all_inputs(PlayerInput player_input);
		void load_game(std::istream &ips);

		void send_packet(Packet &packet);

		void update(void);
		void tick_input_send(void);
		void handle_input_packet(int packet_id, std::istream &packet_ss);
	};
}

#endif /* if !defined(_NET_CLIENT_H) */
