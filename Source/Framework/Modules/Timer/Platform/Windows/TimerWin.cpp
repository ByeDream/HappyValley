/************************************************************************/
/* The Timer Windows Implementation
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "TimerWin.h"

HV::TimerWin::TimerWin(Application *app)
	: Timer(app)
{

}

INT64 HV::TimerWin::getTimeUnitCount() const
{
	INT64 count = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
	return count;
}

INT64 HV::TimerWin::getTimeUnitFrequency() const
{
	INT64 frequency = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	return frequency;
}

