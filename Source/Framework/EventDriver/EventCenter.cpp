/************************************************************************/
/* The Event Manager
/*
/* ByeDream
/* Dec-28-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "EventCenter.h"
#include "EventProcessor.h"

HV::EventCenter::EventCenter()
{

}

HV::EventCenter::~EventCenter()
{

}

void HV::EventCenter::initialize()
{

}

void HV::EventCenter::finitialize()
{

}

void HV::EventCenter::reportEvent(PEvent e)
{

}

void HV::EventCenter::attachProcessor(EventProcessor *processor, EventType *subscriptionList, SIZE_T sizeOfList)
{

}

void HV::EventCenter::detachProcessor(EventProcessor *processor)
{

}

void HV::EventCenter::publishEvent()
{

}

void HV::EventCenter::registerCustomEventType(EventType type)
{

}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, INT64 arg)
{
	Event *e = new Event;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, UINT64 arg)
{
	Event *e = new Event;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, INTPTR arg)
{
	Event *e = new Event;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, DECIMAL arg)
{
	Event *e = new Event;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, INT32 arg0, INT32 arg1)
{
	Event *e = new Event;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, UINT32 arg0, UINT32 arg1)
{
	Event *e = new Event;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, const char *arg)
{
	Event *e = new Event;
	return e;
}
