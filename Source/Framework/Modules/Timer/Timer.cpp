/************************************************************************/
/* The General Interface Of Timer
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "Timer.h"

HV::Timer::Timer(Application *app)
	: Module(app)
	, mIsPaused(false)
	, mSecondPreCount(0.0)
	, mBaseTimeUnitRecord(0)
	, mFrameTimeUnitRecord(0)
	, mPauseTimeUnitRecord(0)
	, mFrameDeltaTimeInSec(0.0)
	, mSuspendableFrameDeltaTimeInSec(0.0)
	, mTotalPausedTimeInSec(0.0)
{

}

void HV::Timer::initialize()
{
	// calculate the second per time count
	mSecondPreCount = 1.0 / static_cast<double>(getTimeUnitFrequency());
}

void HV::Timer::finitialize()
{

}

void HV::Timer::reset()
{
	mIsPaused = false;
	mBaseTimeUnitRecord = getTimeUnitCount();
	mFrameTimeUnitRecord = mBaseTimeUnitRecord;
	mPauseTimeUnitRecord = 0;
	mFrameDeltaTimeInSec = 0.0;
	mSuspendableFrameDeltaTimeInSec = 0.0;
	mTotalPausedTimeInSec = 0.0;
}

void HV::Timer::tick()
{	
	// calculate the frame delta time in second
	INT64 count = getTimeUnitCount();
	mFrameDeltaTimeInSec = static_cast<double>(count - mFrameTimeUnitRecord) * mSecondPreCount;
	mFrameTimeUnitRecord = count;

	// force nonnegative
	mFrameDeltaTimeInSec = mFrameDeltaTimeInSec < 0.0 ? 0.0 : mFrameDeltaTimeInSec;

	mSuspendableFrameDeltaTimeInSec = mIsPaused ? 0.0 : mFrameDeltaTimeInSec;	
}

void HV::Timer::pause()
{
	if (!mIsPaused)
	{
		mPauseTimeUnitRecord = getTimeUnitCount();
		mIsPaused = true;
	}
}

void HV::Timer::resume()
{
	if (mIsPaused)
	{
		// record time elapsed to total paused time
		INT64 count = getTimeUnitCount();
		mTotalPausedTimeInSec += static_cast<double>(count - mPauseTimeUnitRecord) * mSecondPreCount;

		mIsPaused = false;
	}
}

DECIMAL HV::Timer::getFrameDeltaTimeInSecond() const
{
	return static_cast<DECIMAL>(mFrameDeltaTimeInSec);
}

DECIMAL HV::Timer::getSuspendableFrameDeltaTimeInSecond() const
{
	return static_cast<DECIMAL>(mSuspendableFrameDeltaTimeInSec);
}

DECIMAL HV::Timer::getCurrentPausingTimeElapsedInSecond() const
{
	INT64 count = getTimeUnitCount();
	return (mIsPaused ? static_cast<DECIMAL>((count - mPauseTimeUnitRecord) * mSecondPreCount) : 0);
}

DECIMAL HV::Timer::getTotalPausingTimeElapsedInSecond() const
{
	return (static_cast<DECIMAL>(mTotalPausedTimeInSec) + getCurrentPausingTimeElapsedInSecond());
}

DECIMAL HV::Timer::getTotalTimeInSecond() const
{
	INT64 count = getTimeUnitCount();
	return static_cast<DECIMAL>((count - mBaseTimeUnitRecord) * mSecondPreCount);
}

DECIMAL HV::Timer::getSuspendableTotalTimeInSecond() const
{
	DECIMAL time = getTotalTimeInSecond() - getTotalPausingTimeElapsedInSecond();
	return (time < 0 ? 0 : time);
}

DECIMAL HV::Timer::getFPS() const
{
	return (0 == mFrameDeltaTimeInSec ? 0 : static_cast<DECIMAL>(1.0 / mFrameDeltaTimeInSec));
}

// EOF
