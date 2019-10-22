#include "net/client.h"

#include "core/core.h"
#include "core/game.h"
#include "net/net.h"

#include <string>
#include <iostream>
#include <sstream>

using net::Client;

Client::Client(std::istream &ips, std::ostream &ops)
	: net::Node::Node(ips, ops)
{
}

Client::~Client(void)
{

}

Game &Client::game(void)
{
	return m_game;
}

void Client::update(void)
{
	char buf[8192];

	std::streamsize sz = m_ips->readsome(buf, 8192);
	m_input_ss << std::string(buf, sz);
	std::cout << "Client input:" << m_input_ss.str().size() << std::endl;
}
