/************************************************************************/
/* The Base Class Of Event Processor
/*
/* ByeDream
/* Dec-28-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "EventProcessor.h"

#include "EventCenter.h"

HV::EventProcessor::EventProcessor()
{

}

void HV::EventProcessor::_handleEvent(PEvent e)
{
	e->swallowed = handleEvent(e);
	if (mFunc && mOwner)
	{
		e->swallowed = (*mFunc)(e, mOwner);
	}
}

void HV::EventProcessor::registerToEventCenter(EventType *subscriptionList, SIZE_T sizeOfList)
{
	EventCenter::getInstance()->attachProcessor(this, subscriptionList, sizeOfList);
}


