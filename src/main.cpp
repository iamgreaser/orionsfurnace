#include <cstddef>
#include <iostream>

#include <allegro.h>

#include "gfx/gfx.h"
#include "loops/loops.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	cout << "Starting!" << endl;

	allegro_init();

	install_timer();
	install_keyboard();
	install_mouse();

	gfx::init();

	loops::run();

	return 0;
}
END_OF_MAIN()
