/************************************************************************/
/* The Timer Windows Implementation
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#ifndef __TIMER_WIN_H__
#define __TIMER_WIN_H__

#include "Framework/Modules/Timer/Timer.h"

namespace HV
{
	class TimerWin : public Timer
	{
	public:
		TimerWin(Application *app);
		virtual ~TimerWin() {}

	private:
		virtual INT64				getTimeUnitCount() const;			// absolute time unit total counts
		virtual INT64				getTimeUnitFrequency() const;		// time unit counts per second
	};
}

#endif // !__TIMER_H__

// EOF