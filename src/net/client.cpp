#include "net/client.h"

#include "core/core.h"
#include "core/game.h"
#include "net/net.h"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

using net::Client;
using net::GameSnapshotPacket;
using net::GameFramePacket;

Client::Client(std::istream &ips, std::ostream &ops)
	: net::Node::Node(ips, ops)
{
}

Client::~Client(void)
{
	if (m_game != NULL) {
		delete m_game;
		m_game = NULL;
	}
}

Game *Client::game(void)
{
	return m_game;
}

void Client::load_game(std::istream &ips)
{
	if (m_game != NULL) {
		delete m_game;
		m_game = NULL;
	}

	m_game = new Game(ips);
}

void Client::update(void)
{
	// Fetch some input
	{
		char buf[8192];
		std::streamsize sz = m_ips->readsome(buf, 8192);
		m_input_buf += std::string(buf, sz);
	}

	// Extract packets
	for (;;) {
		size_t buffer_size = m_input_buf.size();
		std::cout << "Client input:" << buffer_size << std::endl;

		// Do we have enough stuff to read to make any sense of this?
		if (buffer_size < 5) {
			// No - wait.
			std::cout << "Done " << std::endl;
			break;
		}

		// Extract the bare minimum info needed
		uint32_t sub_size = 0;
		sub_size += ((uint32_t)m_input_buf[0]);
		sub_size += ((uint32_t)m_input_buf[1])<<8;
		sub_size += ((uint32_t)m_input_buf[2])<<16;
		sub_size += ((uint32_t)m_input_buf[3])<<24;
		assert(sub_size >= 1);
		uint8_t packet_id = (uint8_t)m_input_buf[4];
		size_t full_packet_size = 4+1+sub_size;

		// Will it fit?
		if (buffer_size < full_packet_size) {
			// No - leave it in the buffer.
			std::cout << "Overshoot " << std::hex << sub_size << std::dec << std::endl;
			break;
		}

		// Get packet data.
		std::cout << "Client packet:" << full_packet_size << std::endl;
		std::string packet_s(m_input_buf, 4+1, full_packet_size-(4+1));
		m_input_buf.erase(m_input_buf.begin(), m_input_buf.begin()+full_packet_size);
		assert(m_input_buf.size() != buffer_size);
		std::stringstream packet_ss(packet_s);

		// Select by packet ID.
		switch (packet_id)
		{
			case packets::GAME_SNAPSHOT: {
				// Load game.
				std::cout << "Load game" << std::endl;
				this->load_game(packet_ss);
			} break;

			case packets::GAME_FRAME: {
				// Apply frame inputs.
				std::cout << "Apply frame inputs" << std::endl;
				GameFrame game_frame(packet_ss);
				if (m_game != NULL) {
					m_game->tick(game_frame);
				}
			} break;

			default: {
				std::cerr << "ERR: Unhandled packet ID " << std::hex << packet_id << " on client recv" << std::dec << std::endl;
				abort();
			} break;
		}
	}
}
