#ifndef _NET_CLIENT_H
#define _NET_CLIENT_H

#include "core/core.h"
#include "core/game.h"

#include <iostream>
#include <sstream>
#include <string>

namespace net
{
	class Client : public Node
	{
	private:
		Game m_game;
		std::string m_input_buf;
	public:
		Client(std::istream &ips, std::ostream &ops);
		~Client(void);

		Game &game(void);

		void update(void);
	};
}

#endif /* if !defined(_NET_CLIENT_H) */
