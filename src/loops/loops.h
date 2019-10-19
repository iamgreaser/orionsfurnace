#ifndef _LOOPS_LOOPS_H
#define _LOOPS_LOOPS_H

namespace loops
{
	namespace mainloop
	{
		enum MainLoopState
		{
			GAME,

			EXIT,
		};
	}

	using mainloop::MainLoopState;

	void run(void);
}

#endif /* if !defined(_LOOPS_LOOPS_H) */
