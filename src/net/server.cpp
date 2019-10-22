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

void ServerClient::send_packet(net::Packet &packet)
{
	save(*m_ops, packet);
}

Game &Server::game(void)
{
	return m_game;
}

void Server::add_client(std::istream &ips, std::ostream &ops)
{
	m_clients.push_back(ServerClient(ips, ops));
	net::GameSnapshotPacket game_snapshot_packet(m_game);

	// NOTE: Comment the following line out if you wish to test client pre-game-snapshot state.
	m_clients[m_clients.size()-1].send_packet(game_snapshot_packet);
}

void ServerClient::update(void)
{
}

Server::Server(void)
{
	m_game.add_player(Player(&m_game,  4,  8, direction::SOUTH));
	m_game.add_player(Player(&m_game, 10,  6, direction::SOUTH));
}

Server::~Server(void)
{
	if (m_demo_fp != NULL) {
		m_demo_fp->close();
		delete m_demo_fp;
		m_demo_fp = NULL;
	}
}

void Server::broadcast_packet(net::Packet &packet)
{
	// Start recording demo if we haven't yet
	if (m_demo_fp == NULL) {
		m_demo_fp = new std::ofstream("test.demo");
		net::GameSnapshotPacket game_snapshot_packet(m_game);
		save(*m_demo_fp, game_snapshot_packet);
	}

	save(*m_demo_fp, packet);
	for (ServerClient &sc : m_clients) {
		sc.send_packet(packet);
	}
}

void Server::game_tick(GameFrame game_frame)
{
	// Tick game
	m_game.tick(game_frame);

	// Add input to demo
	net::GameFramePacket game_frame_packet(game_frame);
	this->broadcast_packet(game_frame_packet);
}

void Server::quicksave(void)
{
	std::ofstream fp("quick.save");
	save(fp, m_game);
	fp.close();
}

void Server::quickload(void)
{
	std::ifstream fp("quick.save");
	load(fp, m_game);
	fp.close();

	// Broadcast to everyone
	net::GameSnapshotPacket game_snapshot_packet(m_game);
	this->broadcast_packet(game_snapshot_packet);
}

void Server::update(void)
{
	for (ServerClient &sc : m_clients) {
		sc.update();
	}
}
