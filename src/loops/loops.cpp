#include "loops/loops.h"

#include "gfx/gfx.h"
#include "loops/game.h"

#include <allegro.h>

#include <cassert>
#include <cstdlib>
#include <iostream>

using std::cerr;
using std::endl;

namespace loops
{
	int this_tick = 0;
	volatile int next_tick = 0;

	GameLoop game_loop;

	void int_update_tick(void);

	Loop *get_loop_for_state(MainLoopState loop_state);
}

void loops::int_update_tick(void)
{
	next_tick += 1;
}
END_OF_FUNCTION(loops::int_update_tick)

loops::Loop *loops::get_loop_for_state(MainLoopState loop_state)
{
	switch (loop_state) {
		case mainloop::EXIT:
			return NULL;

		case mainloop::GAME:
			return &game_loop;

		default:
			cerr << "FATAL: Unhandled loop state \"" << loop_state << "\"." << endl;
			cerr.flush();
			abort();
			return NULL;
	}
}

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
	for (;;) {
		if (keyboard_needs_poll()) {
			poll_keyboard();
		}

		// TODO: Separate video and logic.
		// For now, this'll have to do.
		if (this_tick >= next_tick) {
			continue;
		}
		this_tick += 1;

		Loop *prev_loop = get_loop_for_state(prev_loop_state);
		Loop *this_loop = get_loop_for_state(loop_state);

		// Deinit and init for loop state transition.
		if (prev_loop != this_loop) {
			if (prev_loop != NULL) {
				prev_loop->deinit();
			}
			if (this_loop != NULL) {
				this_loop->init();
			}
		}

		// If there is no loop, exit.
		if (this_loop == NULL) {
			break;
		}

		// Do tick and draw.
		// FIXME: Separate these two out depending on tick status
		MainLoopState next_loop_state = this_loop->tick();
		this_loop->draw();
		gfx::flip();

		// Update next loop state.
		loop_state = next_loop_state;
	}
}
