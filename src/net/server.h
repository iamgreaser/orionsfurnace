#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#include "core/core.h"
#include "core/game.h"

namespace net
{
	class Server : public Node
	{
	public:
		Server(std::istream &ips, std::ostream &ops);
		~Server(void);
		void update(void);
	};
}

#endif /* if !defined(_NET_SERVER_H) */

