#include "loops/loops.h"

#include "gfx/gfx.h"
#include "loops/game.h"

#include <allegro.h>

#include <iostream>

using std::cerr;
using std::endl;

namespace loops
{
}

void loops::run(void)
{
	MainLoopState loop_state = mainloop::GAME;
	MainLoopState prev_loop_state = mainloop::EXIT;
	while (loop_state != mainloop::EXIT) {
		bool state_changed = (loop_state != prev_loop_state);

		if (keyboard_needs_poll()) {
			poll_keyboard();
		}

		switch (loop_state) {
			case mainloop::GAME:
				loop_state = game_loop(state_changed);
				break;
			default:
				cerr << "FATAL: Unhandled loop state \"" << loop_state << "\"." << endl;
				cerr.flush();
				abort();
				break;
		}

		gfx::flip();
	}
}
