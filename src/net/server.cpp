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
	if (m_demo_fp != NULL) {
		m_demo_fp->close();
		delete m_demo_fp;
		m_demo_fp = NULL;
	}
}

void Server::game_tick(GameFrame game_frame)
{
	// Start recording demo if we haven't yet
	if (m_demo_fp == NULL) {
		m_demo_fp = new std::ofstream("test.demo");
		net::GameSnapshotPacket game_snapshot_packet(m_game);
		save(*m_demo_fp, game_snapshot_packet);
	}

	// Tick game
	m_game.tick(game_frame);

	// Add input to demo
	net::GameFramePacket game_frame_packet(game_frame);
	save(*m_demo_fp, game_frame_packet);

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

	// Also load in demo
	if (m_demo_fp == NULL) {
		m_demo_fp = new std::ofstream("test.demo");
	}
	net::GameSnapshotPacket game_snapshot_packet(m_game);
	save(*m_demo_fp, game_snapshot_packet);
}

void Server::update(void)
{
	for (ServerClient &sc : m_clients) {
		sc.update();
	}
}
