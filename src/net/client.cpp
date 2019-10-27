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

#include "net/client.h"

#include "core/core.h"
#include "core/helpers.h"
#include "core/game.h"
#include "net/net.h"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

using net::Client;
using net::GameSnapshotPacket;
using net::GameFramePacket;


Client::Client(std::shared_ptr<net::PipeEnd> pipe_end)
  : net::Node::Node(pipe_end)
{
}

Client::~Client(void)
{
  if (m_game != nullptr) {
    delete m_game;
    m_game = nullptr;
  }
}

Game *Client::game(void)
{
  return m_game;
}

int Client::get_player_idx(void)
{
  return m_player_idx;
}

PlayerInput Client::get_player_input(void)
{
  return m_player_input;
}

void Client::set_all_inputs(PlayerInput player_input)
{
  m_player_input = player_input;
}

std::string Client::get_central_message(void)
{
  switch (m_status)
  {
    case client_status::SENDING_HELLO:
    case client_status::LOADING_GAME:
      return "Loading...";

    case client_status::PLAYING_GAME:
      return "";

    case client_status::DISCONNECTED:
      return "Disconnected:\n\n" + m_disconnect_message;

    default:
      PANIC("LOGIC ERROR");
      break;
  }
}

void Client::load_game(std::istream &ips)
{
  if (m_game != nullptr) {
    delete m_game;
    m_game = nullptr;
  }

  m_game = new Game(ips);
}

void Client::update(void)
{
  m_pipe_end->pump_recv();

  this->update_packets();

  // If we're disconnected, set a generic message.
  if (m_pipe_end->is_disconnected()) {
    if (m_status != client_status::DISCONNECTED) {
      m_disconnect_message = m_pipe_end->get_disconnect_message();
      m_status = client_status::DISCONNECTED;
    }
  }

  switch (m_status)
  {
    case client_status::SENDING_HELLO: {
      // Send a Hello packet.
      // TODO: Nicknames
      net::HelloPacket hello_packet = net::HelloPacket(net::ClientHello());
      this->send_packet(hello_packet);

      // Now we move to the next awaiting state.
      m_status = client_status::LOADING_GAME;
    } break;

    case client_status::LOADING_GAME:
      // All we can do here really is wait.
      // Until we've got everything we need that is...
      if (this->m_game == nullptr) { break; }
      if (this->m_player_idx < 0) { break; }

      // ALRIGHT WE'RE GOOD NOW
      this->m_status = client_status::PLAYING_GAME;
      break;

    case client_status::PLAYING_GAME:
      // Now we can spam inputs everywhere it'll be great
      this->tick_input_send();
      break;

    case client_status::DISCONNECTED:
      // Welp, time to show a message telling people to quit.
      // TODO: implement that in the gameloop.
      break;

    default:
      PANIC("LOGIC ERROR");
      break;
  }

  m_pipe_end->pump_send();
}

void Client::tick_input_send(void)
{
  if (m_ready_to_send_input) {
    net::ProvideInputPacket provide_input_packet(m_player_input);
    this->send_packet(provide_input_packet);
    m_ready_to_send_input = false;
  }
}

void Client::handle_input_packet(int packet_id, std::istream &packet_ss)
{
  // Select by packet ID.
  switch (packet_id)
  {
    case packets::DISCONNECT: {
      // Disconnect ourselves.
      load(packet_ss, m_disconnect_message);
      m_status = client_status::DISCONNECTED;
    } break;

    case packets::THIS_IS_YOU: {
      // Load player index.
      std::cout << "Load player index" << std::endl;
      int16_t player_idx = -1;
      load(packet_ss, player_idx);
      m_player_idx = player_idx;
    } break;

    case packets::GAME_SNAPSHOT: {
      // Load game.
      std::cout << "Load game" << std::endl;
      this->load_game(packet_ss);
      assert(m_game != nullptr);
    } break;

    case packets::GAME_FRAME: {
      // Apply frame inputs.
      //std::cout << "Apply frame inputs" << std::endl;
      GameFrame game_frame(packet_ss);
      if (m_game != nullptr) {
        m_game->tick(game_frame);
      }
      m_ready_to_send_input = true;
    } break;

    case packets::ADD_PLAYER: {
      // Add the new player
      std::cout << "Add new player" << std::endl;
      PlayerAdd player_add(packet_ss);
      if (m_game != nullptr) {
        assert(player_add.get_player_idx() == m_game->get_player_count());
        m_game->add_player(player_add.make_player(m_game));
      }
    } break;

    default: {
      std::cerr << "ERR: Unhandled packet ID " << std::hex << packet_id << " on client recv" << std::dec << std::endl;
      abort();
    } break;
  }
}
