#include "net/server.h"

#include "core/core.h"
#include "core/game.h"
#include "net/net.h"

using net::Server;

Server::Server(std::istream &ips, std::ostream &ops)
	: net::Node::Node(ips, ops)
{
}

Server::~Server(void)
{

}

void Server::update(void)
{

}
