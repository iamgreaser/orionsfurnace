#ifndef _NET_CLIENT_H
#define _NET_CLIENT_H

#include "core/core.h"
#include "core/game.h"

#include <iostream>
#include <sstream>

namespace net
{
	class Client : public Node
	{
	private:
		Game m_game;
		std::stringstream m_input_ss;
	public:
		Client(std::istream &ips, std::ostream &ops);
		~Client(void);

		Game &game(void);

		void update(void);
	};
}

#endif /* if !defined(_NET_CLIENT_H) */
