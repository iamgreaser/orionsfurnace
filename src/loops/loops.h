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

#ifndef LOOPS_LOOPS_H
#define LOOPS_LOOPS_H

#include <string>

namespace loops
{
  namespace launch_mode
  {
    enum LaunchMode
    {
      PLAYER_SERVER = 0,
      CLIENT,
      //HEADLESS_SERVER,
    };
  }
  using launch_mode::LaunchMode;

  namespace mainloop
  {
    enum MainLoopState
    {
      GAME,

      EXIT,
    };
  }

  using mainloop::MainLoopState;

  class Loop
  {
  public:
    virtual void init(void) {};
    virtual void deinit(void) {};
    virtual MainLoopState tick(void) = 0;
    virtual void draw(void) = 0;
  };

  constexpr int TICKS_PER_SECOND = 60;

  void run(loops::LaunchMode launch_mode, std::string net_addr, int net_port);
}

#endif /* if !defined(LOOPS_LOOPS_H) */
