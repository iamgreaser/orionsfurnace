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

#ifndef NET_NET_H
#define NET_NET_H

#include "core/core.h"
#include "core/save.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <sstream>

namespace packets
{
	enum PacketType
	{
		DISCONNECT = 0x00, // C and S // TODO!
		HELLO = 0x01, // C // TODO!
		THIS_IS_YOU = 0x02, // S // TODO!
		PROVIDE_INPUT = 0x10, // C // TODO!
		GAME_SNAPSHOT = 0x20, // S
		GAME_FRAME = 0x30, // S
		ADD_PLAYER = 0x40, // S
	};
}

using packets::PacketType;

namespace net
{
	class Client;
	class ClientHello;
	class Packet;
	class PipeEnd;
	class Server;

	class PipeEnd
	{
	public:
		virtual ~PipeEnd(void) {}
		virtual std::ostream &send_stream(void) = 0;
		virtual std::istream &recv_stream(void) = 0;
		virtual void pump_recv(void) {}
		virtual void pump_send(void) {}
	};


	class Packet : public Saveable
	{
	public:
		virtual uint8_t get_packet_id(void) = 0;
		//virtual void load_this(std::istream &ips) = 0;
		//virtual void save_this(std::ostream &ops) = 0;
	};

	template <uint8_t PID, class T>
	class ClassPacket : public Packet
	{
	private:
		T m_value;
	public:
		ClassPacket(std::istream &ips)
		{
			load(ips, *this);
		}

		ClassPacket(T value)
			: m_value(value)
		{
		}

		T get_value(void)
		{
			return m_value;
		}

		uint8_t get_packet_id(void) override
		{
			return PID;
		}

		void load_this(std::istream &ips) override
		{
			// Load datagram
			uint32_t packet_length = 0;
			load(ips, packet_length);
			uint8_t packet_id = PID^1;
			load(ips, packet_id);
			assert(packet_id == PID);

			// Load datagram buffer
			char *buf = new char[packet_length];
			ips.read(buf, packet_length);
			std::string str(buf, packet_length);
			delete[] buf;

			// Load datagram contents
			std::stringstream ss(str);
			load(ss, m_value);
		}

		void save_this(std::ostream &ops) override
		{
			// Build datagram contents
			uint8_t packet_id = PID;
			std::stringstream ss;
			save(ss, m_value);

			// Save datagram
			std::string str = ss.str();
			size_t raw_packet_length = str.size();
			assert(raw_packet_length <= (size_t)(0xFFFFFFFFULL));
			uint32_t packet_length = (uint32_t)raw_packet_length;
			save(ops, packet_length);
			save(ops, packet_id);
			ops << str;
		}
	};

	typedef class ClassPacket<packets::DISCONNECT, std::string> DisconnectPacket;
	typedef class ClassPacket<packets::HELLO, ClientHello> HelloPacket;
	typedef class ClassPacket<packets::THIS_IS_YOU, uint16_t> ThisIsYouPacket;
	typedef class ClassPacket<packets::GAME_SNAPSHOT, Game> GameSnapshotPacket;
	typedef class ClassPacket<packets::GAME_FRAME, GameFrame> GameFramePacket;
	typedef class ClassPacket<packets::PROVIDE_INPUT, PlayerInput> ProvideInputPacket;
	typedef class ClassPacket<packets::ADD_PLAYER, PlayerAdd> AddPlayerPacket;

	class Node
	{
	protected:
		net::PipeEnd *m_pipe_end;
		std::string m_input_buf;
	public:
		Node(net::PipeEnd *pipe_end);
		void send_packet(net::Packet &packet);
		virtual ~Node(void);
		virtual void update(void) = 0;
		void update_packets(void);
		virtual void handle_input_packet(int packet_id, std::istream &packet_ss) = 0;
	};

	class ClientHello: public Saveable
	{
	private:
		std::string m_version;
		std::string m_nickname;
	public:
		ClientHello(std::string nickname = "");
		ClientHello(std::istream &ips);

		std::string get_version(void) {
			return m_version;
		}
		bool is_current_version(void);

		void load_this(std::istream &ips) override;
		void save_this(std::ostream &ops) override;
	};

}

#endif /* if !defined(NET_NET_H) */
