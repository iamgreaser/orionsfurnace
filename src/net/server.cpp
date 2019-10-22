#include "net/server.h"

#include "core/core.h"
#include "core/game.h"
#include "net/net.h"

using net::Server;
using net::ServerClient;

ServerClient::ServerClient(std::istream &ips, std::ostream &ops)
	: net::Node::Node(ips, ops)
{
}

ServerClient::~ServerClient(void)
{

}

Game &Server::game(void)
{
	return m_game;
}

void Server::add_client(std::istream &ips, std::ostream &ops)
{
	m_clients.push_back(ServerClient(ips, ops));
}

void ServerClient::update(void)
{

}

Server::Server(void)
{
}

Server::~Server(void)
{
}

void Server::game_tick(GameFrame game_frame)
{
	m_game.tick(game_frame);
}

void Server::update(void)
{
	for (ServerClient &sc : m_clients) {
		sc.update();
	}
}
