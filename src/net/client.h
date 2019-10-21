#ifndef _NET_CLIENT_H
#define _NET_CLIENT_H

#include "core/core.h"
#include "core/game.h"

namespace net
{
	class Client : public Node
	{
	public:
		Client(std::istream &ips, std::ostream &ops);
		~Client(void);
		void update(void);
	};
}

#endif /* if !defined(_NET_CLIENT_H) */
