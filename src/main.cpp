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

#include <cstddef>
#include <iostream>
#include <signal.h>

#include <SDL.h>

#include "core/version.h"
#include "gfx/gfx.h"
#include "loops/loops.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

void show_help(char *arg0)
{
  cerr << "Orion's Furnace v" << get_engine_version() << endl;
  cerr << "Copyright (C) 2019 Ben \"GreaseMonkey\" Russell & contributors" << endl;
  cerr << endl;
  cerr << "This program is free software: you can redistribute it and/or modify" << endl;
  cerr << "it under the terms of the GNU Affero General Public License as published by" << endl;
  cerr << "the Free Software Foundation, either version 3 of the License, or" << endl;
  cerr << "(at your option) any later version." << endl;
  cerr << endl;
  cerr << "This program is distributed in the hope that it will be useful," << endl;
  cerr << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
  cerr << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl;
  cerr << "GNU Affero General Public License for more details." << endl;
  cerr << endl;
  cerr << "You should have received a copy of the GNU Affero General Public License" << endl;
  cerr << "along with this program.  If not, see <https://www.gnu.org/licenses/>." << endl;
  cerr << endl;
  cerr << "Usage:" << endl;
  cerr << "    " << string(arg0) << " --server 13420" << endl;
  cerr << "    " << string(arg0) << " --client localhost 13420" << endl;
  cerr << endl;
  cerr << "Arguments:" << endl;
  cerr << "    --server PORT" << endl;
  cerr << "        Starts a server at TCP port PORT." << endl;
  cerr << endl;
  cerr << "    --client ADDR PORT" << endl;
  cerr << "        Starts a client connecting to IPv4/v6 address ADDR at TCP port PORT." << endl;
  cerr << endl;
}

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  // Parse arguments
  loops::LaunchMode launch_mode = loops::launch_mode::PLAYER_SERVER;
  bool did_set_launch_mode = false;
  bool bailed_inside = false;
  string net_addr = "localhost";
  int net_port = -1;

  for (int i = 1; i < argc;) {
    bailed_inside = true;

    if (string(argv[i]) == "--server") {
      if (did_set_launch_mode) {
        cerr << "Attempted to set launch mode twice" << endl;
        break;
      }

      if (i+2 > argc) {
        cerr << "Not enough arguments for --server" << endl;
        break;
      }

      did_set_launch_mode = true;
      launch_mode = loops::launch_mode::PLAYER_SERVER;
      net_port = std::stoi(string(argv[i+1]));
      i += 2;

    } else if (string(argv[i]) == "--client") {
      if (did_set_launch_mode) {
        cerr << "Attempted to set launch mode twice" << endl;
        break;
      }

      if (i+3 > argc) {
        cerr << "Not enough arguments for --client" << endl;
        break;
      }

      did_set_launch_mode = true;
      launch_mode = loops::launch_mode::CLIENT;
      net_addr = string(argv[i+1]);
      net_port = std::stoi(string(argv[i+2]));
      i += 3;

    } else {
      cerr << "Invalid argument \"" << string(argv[i]) << "\"" << endl;
      break;
    }

    bailed_inside = false;
  }

  if ((!did_set_launch_mode) || bailed_inside) {
    show_help(argv[0]);
    return 2;
  }

  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

  // Even after all these years, SDL *still* does this bullshit I have to undo. --GM
  signal(SIGINT,  SIG_DFL);
  signal(SIGTERM, SIG_DFL);

  gfx::init();

  loops::run(launch_mode, net_addr, net_port);

  return 0;
}
