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

#ifndef _NET_PIPE_H
#define _NET_PIPE_H

#include "net/net.h"

#include <iostream>
#include <sstream>

namespace net
{
	class RawPipeEnd : public PipeEnd
	{
	protected:
		std::stringstream *m_send_ss = NULL;
		std::stringstream *m_recv_ss = NULL;
	public:
		RawPipeEnd(std::stringstream *ips, std::stringstream *ops)
			: m_send_ss(ips)
			, m_recv_ss(ops)
		{
			m_send_ss = ips;
			m_recv_ss = ops;
		}
		std::ostream &send_stream(void) {
			return static_cast<std::ostream&>(*m_send_ss);
		}
		std::istream &recv_stream(void) {
			return static_cast<std::istream&>(*m_recv_ss);
		}
	};

	class RawPipe
	{
	protected:
		std::stringstream m_a_to_b;
		std::stringstream m_b_to_a;
		RawPipeEnd m_end_a;
		RawPipeEnd m_end_b;
	public:
		RawPipe(void)
			: m_end_a(&m_b_to_a, &m_a_to_b)
			, m_end_b(&m_a_to_b, &m_b_to_a)
		{
		}
		RawPipeEnd &end_a(void) { return m_end_a; }
		RawPipeEnd &end_b(void) { return m_end_b; }
	};
}

#endif /* if !defined(_NET_PIPE_H) */
