#ifndef _NET_NET_H
#define _NET_NET_H

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
	};
}

using packets::PacketType;

namespace net
{
	class Packet : public Saveable
	{
	public:
		virtual uint8_t get_packet_id(void) = 0;
		virtual void load_this(std::istream &ips) = 0;
		virtual void save_this(std::ostream &ops) = 0;
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

		uint8_t get_packet_id(void)
		{
			return PID;
		}

		void load_this(std::istream &ips)
		{
			// Load datagram
			uint32_t packet_length = 0;
			load(ips, packet_length);

			// Load datagram buffer
			char *buf = new char[packet_length];
			ips.read(buf, packet_length);
			std::string str(buf, packet_length);
			delete buf;

			// Load datagram contents
			std::stringstream ss(str);
			uint8_t packet_id = PID^1;
			load(ss, packet_id);
			assert(packet_id == PID);
			load(ss, m_value);
		}

		void save_this(std::ostream &ops)
		{
			// Build datagram contents
			uint8_t packet_id = PID;
			std::stringstream ss;
			save(ss, packet_id);
			save(ss, m_value);

			// Save datagram
			std::string str = ss.str();
			uint32_t packet_length = str.size();
			save(ops, packet_length);
			ops << str;
		}
	};

	typedef class ClassPacket<packets::GAME_SNAPSHOT, Game> GameSnapshotPacket;
	typedef class ClassPacket<packets::GAME_FRAME, GameFrame> GameFramePacket;

	class Node
	{
	protected:
		std::istream *m_ips;
		std::ostream *m_ops;
	public:
		Node(std::istream &ips, std::ostream &ops);
		virtual ~Node(void);
		virtual void update(void) = 0;
	};
}

#endif /* if !defined(_NET_NET_H) */
