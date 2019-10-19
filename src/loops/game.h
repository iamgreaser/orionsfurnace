#ifndef _LOOPS_GAME_H
#define _LOOPS_GAME_H

#include "loops/loops.h"

namespace loops
{
	class GameLoop : public Loop
	{
	public:
		MainLoopState tick(void);
		void draw(void);
	};
}

#endif /* if !defined(_LOOPS_GAME_H) */
