/************************************************************************/
/* The General Interface Of Timer
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#include "Framework/Modules/Module.h"

namespace HV
{
	class Timer : public Module
	{
	public:
		Timer(Application *app);
		virtual ~Timer() {}

		virtual void				initialize();
		virtual void				finitialize();

		virtual void				reset();
		virtual void				tick();

		virtual void				pause();
		virtual void				resume();

		virtual DECIMAL				getFrameDeltaTimeInSecond() const;
		virtual DECIMAL				getSuspendableFrameDeltaTimeInSecond() const;

		virtual DECIMAL				getCurrentPausingTimeElapsedInSecond() const;
		virtual DECIMAL				getTotalPausingTimeElapsedInSecond() const;

		virtual DECIMAL				getTotalTimeInSecond() const;
		virtual DECIMAL				getSuspendableTotalTimeInSecond() const;

		virtual DECIMAL				getFPS() const;

	protected:
		virtual INT64				getTimeUnitCount() const = 0;			// absolute time unit total counts
		virtual INT64				getTimeUnitFrequency() const = 0;		// time unit counts per second

		bool						mIsPaused;
		double						mSecondPreCount;

		INT64						mBaseTimeUnitRecord;
		INT64						mFrameTimeUnitRecord;
		INT64						mPauseTimeUnitRecord;

		double						mFrameDeltaTimeInSec;
		double						mSuspendableFrameDeltaTimeInSec;
		double						mTotalPausedTimeInSec;
	};

	inline UINT32					_sec2ms(DECIMAL sec)
	{
		return static_cast<UINT32>(sec * 1000);
	}

	inline DECIMAL					_ms2sec(UINT32 ms)
	{
		return static_cast<DECIMAL>(static_cast<double>(ms) / 1000);
	}
}

#endif // !__TIMER_H__
