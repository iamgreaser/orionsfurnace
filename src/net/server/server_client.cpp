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

#include "net/server/server_client.h"

#include "core/helpers.h"
#include "core/version.h"
#include "net/net.h"
#include "net/packet/packet.h"
#include "net/server/server.h"

using net::ServerClient;

ServerClient::ServerClient(Server *server, int player_index, std::shared_ptr<net::PipeEnd> pipe_end)
  : net::Node::Node(pipe_end)
  , m_server(server)
  , m_player_index(player_index)
{
  std::cout << "New ServerClient " << reinterpret_cast<void *>(this) << std::endl;
}

ServerClient::~ServerClient(void)
{
  std::cout << "Deleting ServerClient " << reinterpret_cast<void *>(this) << std::endl;
}

void ServerClient::update(void)
{
  m_pipe_end->pump_recv();

  this->update_packets();

  m_pipe_end->pump_send();
}

void ServerClient::handle_input_packet(int packet_id, std::istream &packet_ss)
{
  // Ignore if disconnected.
  if (m_disconnected) {
    return;
  }

  // Select by packet ID.
  switch (packet_id)
  {
    case packets::HELLO: {
      // Say hello!
      std::cout << "Hello from client" << std::endl;

      ClientHello hello(packet_ss);

      // Is this the correct version?
      if (!hello.is_current_version()) {
        // No - disconnect the client
        std::cout << "Client has the wrong version!" << std::endl;

        net::DisconnectPacket disconnect_packet(
          std::string("Version mismatch!\n")
          + std::string("Please use v")
          + get_engine_version()
        );
        this->send_packet(disconnect_packet);
        m_disconnected = true;
        break;
      }

      std::shared_ptr<Game> game = m_server->game();
      m_player_index = game.get()->get_player_count();

      // TODO: Add spawn points
      // FIXME: This COULD spawn one player atop another, or atop a wall or something!
      // (but at least it terminates)
      int cx = static_cast<int>(game.get()->random().next_int(
        static_cast<uint32_t>(game.get()->get_width())));
      int cy = static_cast<int>(game.get()->random().next_int(
        static_cast<uint32_t>(game.get()->get_height())));
      for (int i = 0; i < 100; i++) {
        if (game.get()->can_step_into(cx, cy, true)) {
          // Our position is good!
          break;
        }
        cx = static_cast<int>(game.get()->random().next_int(
          static_cast<uint32_t>(game.get()->get_width())));
        cy = static_cast<int>(game.get()->random().next_int(
          static_cast<uint32_t>(game.get()->get_height())));
      }

      // Add the player
      game.get()->add_player(Player(game.get(), cx, cy, direction::SOUTH));

      // Add the player to everyone else
      // TODO: Cut down on redundancy here and use the same Player object
      net::AddPlayerPacket add_player_packet(PlayerAdd(
        m_player_index, cx, cy, direction::SOUTH));
      m_server->broadcast_packet(add_player_packet);

      // Send a game snapshot to the client
      // Also send a "This Is You" packet
      assert(m_player_index >= 0);
      assert(m_player_index <= 0xFFFF);
      uint16_t saved_player_index = static_cast<uint16_t>(m_player_index);
      net::ThisIsYouPacket this_is_you_packet(saved_player_index);
      net::GameSnapshotPacket game_snapshot_packet(*game.get());
      this->send_packet(this_is_you_packet);
      this->send_packet(game_snapshot_packet);
    } break;

    case packets::PROVIDE_INPUT: {
      // Set frame input.
      //std::cout << "Provide input for player " << m_player_index << std::endl;
      assert(m_server != nullptr);
      PlayerInput player_input(packet_ss);
      m_player_input = player_input;
    } break;

    default: {
      std::cerr << "ERR: Unhandled packet ID " << std::hex << packet_id << " on server recv" << std::dec << std::endl;
      PANIC("Unhandled packet ID");
    } break;
  }
}

