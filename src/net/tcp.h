/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _NET_TCP_H
#define _NET_TCP_H

#include "net/net.h"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace net
{
	class TCPPipeEnd : public PipeEnd
	{
	protected:
		int m_sockfd;
		std::stringstream m_send_ss;
		std::stringstream m_recv_ss;
		std::string m_recv_buf = "";
		std::string m_send_buf = "";
	public:
		TCPPipeEnd(int sockfd);
		TCPPipeEnd(std::string addr, int port);
		~TCPPipeEnd(void);

		std::ostream &send_stream(void) {
			return static_cast<std::ostream&>(m_send_ss);
		}
		std::istream &recv_stream(void) {
			return static_cast<std::istream&>(m_recv_ss);
		}

		void pump_recv(void);
		void pump_send(void);
	};

	class TCPServer
	{
	protected:
		int m_sockfd;
	public:
		TCPServer(int port);
		~TCPServer(void);

		TCPPipeEnd *accept_if_available(void);

	protected:
		bool is_good_ai_family(int family);
	};
}

#endif /* if !defined(_NET_TCP_H) */

