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

	loops::run();

	return 0;
}
