#include "loops/loops.h"

#include "gfx/gfx.h"
#include "loops/game.h"

#include <allegro.h>

#include <cassert>
#include <iostream>

using std::cerr;
using std::endl;

namespace loops
{
	int this_tick = 0;
	volatile int next_tick = 0;

	void int_update_tick(void);
}

void loops::int_update_tick(void)
{
	next_tick += 1;
}
END_OF_FUNCTION(loops::int_update_tick)

void loops::run(void)
{
	// Install timer interrupt hook.
	//
	// OK, this used to literally be a timer interrupt hook.
	// And if this ever gets a DOS port (read: it probably won't),
	// it could still literally be a timer interrupt hook.
	//
	// Anyway, this pattern is standard in Allegro 4.
	// I'd like to read the time instead,
	// but Allegro doesn't appear to provide that.
	//
	// Meanwhile, SDL gives you SDL_GetTicks,
	// which unfortunately has 100Hz granularity.
	//
	// This gives you 1193181Hz granularity.
	// Slightly overkill and probably not that accurate,
	// but comparable to using gettimeofday.
	//
	// Which might not be usable on Windows...
	//
	// C++11 has some offerings which may be more appropriate.
	// std::chrono::steady_timer is looking promising.
	//
	int install_int_result = install_int_ex(
		int_update_tick,
		BPS_TO_TIMER(TICKS_PER_SECOND));
	assert(install_int_result == 0);

	MainLoopState loop_state = mainloop::GAME;
	MainLoopState prev_loop_state = mainloop::EXIT;
	while (loop_state != mainloop::EXIT) {
		bool state_changed = (loop_state != prev_loop_state);

		if (keyboard_needs_poll()) {
			poll_keyboard();
		}

		// TODO: Separate video and logic.
		// For now, this'll have to do.
		if (this_tick >= next_tick) {
			continue;
		}
		this_tick += 1;

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
