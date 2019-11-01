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

#include "net/server/server.h"

#include "core/core.h"
#include "core/game.h"
#include "core/player.h"
#include "core/version.h"
#include "net/net.h"
#include "net/packet/packet.h"
#include "net/server/server_client.h"

using net::ServerClient;
using net::Server;

////////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_tcp_server(port)
{
}

Server::~Server(void)
{
}

void Server::add_client(std::shared_ptr<net::PipeEnd> pipe_end)
{
  //int client_index = m_clients.size();
  // Add a new client
  m_clients.push_back(std::shared_ptr<ServerClient>(
    new ServerClient(this, -1, pipe_end)));
}

int Server::register_new_player(void)
{
  int player_idx = m_next_player_idx;
  m_next_player_idx += 1;

  return player_idx;
}

void Server::broadcast_packet(net::Packet &packet)
{
  // Start recording demo if we haven't yet
  if (m_demo_fp == nullptr) {
    m_demo_fp.reset(new std::ofstream("test.demo"));
    net::GameSnapshotPacket game_snapshot_packet(*m_game.get());
    save(*m_demo_fp.get(), game_snapshot_packet);
  }

  save(*m_demo_fp.get(), packet);
  for (std::shared_ptr<ServerClient> sc : m_clients) {
    sc.get()->send_packet(packet);
  }
}

void Server::broadcast_packet_ignoring_client(net::Packet &packet, ServerClient *ignore_sc)
{
  // Start recording demo if we haven't yet
  if (m_demo_fp == nullptr) {
    m_demo_fp.reset(new std::ofstream("test.demo"));
    net::GameSnapshotPacket game_snapshot_packet(*m_game.get());
    save(*m_demo_fp.get(), game_snapshot_packet);
  }

  save(*m_demo_fp.get(), packet);
  for (std::shared_ptr<ServerClient> sc : m_clients) {
    if (sc.get() != ignore_sc) {
      sc.get()->send_packet(packet);
    }
  }
}

void Server::game_tick(GameFrame game_frame)
{
  // Tick game
  m_game.get()->tick(game_frame);

  // Add input to demo
  net::GameFramePacket game_frame_packet(game_frame);
  this->broadcast_packet(game_frame_packet);
}

void Server::quicksave(void)
{
  std::ofstream fp("quick.save");
  save(fp, *m_game.get());
  fp.close();
}

void Server::quickload(void)
{
  std::ifstream fp("quick.save");
  load(fp, *m_game.get());
  fp.close();

  // Broadcast to everyone
  net::GameSnapshotPacket game_snapshot_packet(*m_game.get());
  this->broadcast_packet(game_snapshot_packet);
}

void Server::update(void)
{
  // Add a client if one is trying to connect
  std::shared_ptr<net::TCPPipeEnd> pipe_end = m_tcp_server.accept_if_available();
  if (pipe_end != nullptr) {
    std::cout << "Accepting new client" << std::endl;
    this->add_client(pipe_end);
  }

  // Form a frame
  GameFrame game_frame;

  for (int i = 0; i < m_next_player_idx; i++) {
    game_frame.player_set_all_inputs(
      i, m_clients[i]->get_player_input());
  }

  this->game_tick(game_frame);
  for (std::shared_ptr<ServerClient> sc : m_clients) {
    sc.get()->update();
  }
}
