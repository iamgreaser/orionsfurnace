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

#include "gfx/gfx.h"
#include "loops/loops.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	cout << "Starting!" << endl;

	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

	// Even after all these years, SDL *still* does this bullshit I have to undo. --GM
	signal(SIGINT,  SIG_DFL);
	signal(SIGTERM, SIG_DFL);

	gfx::init();

	loops::run(
		loops::launch_mode::PLAYER_SERVER,
		//loops::launch_mode::CLIENT,
		"localhost",
		13420);

	return 0;
}
