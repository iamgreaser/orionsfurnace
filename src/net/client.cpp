#include "net/client.h"

#include "core/core.h"
#include "core/game.h"
#include "net/net.h"

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

}
