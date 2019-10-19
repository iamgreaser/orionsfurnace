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

	class Loop
	{
	public:
		virtual void init(void);
		virtual void deinit(void);
		virtual MainLoopState tick(void) = 0;
		virtual void draw(void) = 0;
	};

	constexpr int TICKS_PER_SECOND = 60;

	void run(void);
}

#endif /* if !defined(_LOOPS_LOOPS_H) */
