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

#include "loops/loops.h"

#include "gfx/gfx.h"
#include "loops/game.h"

#include <SDL.h>

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

using std::chrono::time_point;
using std::chrono::microseconds;
using std::chrono::steady_clock;

namespace loops
{
  int this_tick = 0;

  GameLoop game_loop;

  void int_update_tick(void);

  Loop *get_loop_for_state(MainLoopState loop_state);
}

loops::Loop *loops::get_loop_for_state(MainLoopState loop_state)
{
  switch (loop_state) {
    case mainloop::EXIT:
      return nullptr;

    case mainloop::GAME:
      return &game_loop;

    default:
      std::cerr << "FATAL: Unhandled loop state \"" << loop_state << "\"." << std::endl;
      std::cerr.flush();
      abort();
      return nullptr;
  }
}

void loops::run(loops::LaunchMode launch_mode, std::string net_addr, int net_port)
{
  // TODO: Use net_addr
  (void)net_addr;

  time_point<steady_clock> time_now = steady_clock::now();
  time_point<steady_clock> time_next = time_now;
  constexpr microseconds time_step(1000000/TICKS_PER_SECOND);

  switch (launch_mode)
  {
    case loops::launch_mode::PLAYER_SERVER:
      game_loop.start_server(net_port);
      game_loop.start_client("", net_port);
      break;

    case loops::launch_mode::CLIENT:
      game_loop.start_client(net_addr, net_port);
      break;

    default: assert(!"LOGIC ERROR"); break;
  }

  MainLoopState loop_state = mainloop::GAME;
  MainLoopState prev_loop_state = mainloop::EXIT;
  for (;;) {
    // Get our loops.
    Loop *prev_loop = get_loop_for_state(prev_loop_state);
    Loop *this_loop = get_loop_for_state(loop_state);

    // Deinit and init for loop state transition.
    if (prev_loop != this_loop) {
      if (prev_loop != nullptr) {
        prev_loop->deinit();
      }
      if (this_loop != nullptr) {
        this_loop->init();
      }
    }

    // Update the previous loop state, and track the next.
    prev_loop_state = loop_state;
    MainLoopState next_loop_state = loop_state;

    // If there is no loop, exit.
    if (this_loop == nullptr) {
      break;
    }

    // Update time.
    time_now = steady_clock::now();

    // Check if we have anything to tick.
    if (time_next <= time_now) {
      // Tick it.
      next_loop_state = this_loop->tick();
      this_tick += 1;
      time_next += time_step;
    }

    // Draw if we're fast enough.
    if (time_now < time_next) {
      // Draw and flip.
      this_loop->draw();
      gfx::flip();

      // Idle if we can.
      time_now = steady_clock::now();
      if (time_now < time_next) {
        std::this_thread::sleep_for(time_next - time_now);
      }
    }

    // Update next loop state.
    loop_state = next_loop_state;
  }
}
