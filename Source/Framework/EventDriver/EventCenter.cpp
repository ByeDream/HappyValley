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

#define EVENT_POOL_SIZE 1024

HV::EventCenter::EventCenter()
{

}

HV::EventCenter::~EventCenter()
{

}

void HV::EventCenter::initialize()
{
	// init event pool
	mEventPool.reserve(EVENT_POOL_SIZE);

	// init subscription list
	for (UINT32 i = 0; i < EVENT_COUNT; i++)
	{
		mSubscriptionList[i] = new ProcessorContactThin();
	}
}

void HV::EventCenter::finitialize()
{
	for (SubscriptionList::iterator itor; itor != mSubscriptionList.end(); itor++)
	{
		SAFE_DELETE(itor->second);
	}
	mSubscriptionList.clear();

	clearEventPool();
}

void HV::EventCenter::reportEvent(PEvent e)
{
	if (NULL == e)
	{
		THROW_EXCEPTION(SimpleException, "PEvent should not be NULL");
	}

	if (std::find(mEventPool.begin(), mEventPool.end(), e) == mEventPool.end())
	{
		mEventPool.push_back(e);
	}
}

void HV::EventCenter::attachProcessor(EventProcessor *processor, EventType *subscriptionList, SIZE_T sizeOfList)
{
	if (NULL == processor)
	{
		THROW_EXCEPTION(SimpleException, "processor pointer should not be NULL");
	}

	if (NULL != subscriptionList)
	{
		for (SIZE_T i = 0; i < sizeOfList; i++)
		{
			EventType subscriptionType = subscriptionList[i];
			// non - common
			if (EVENT_COMMON != subscriptionType)
			{
				SubscriptionList::iterator itor = mSubscriptionList.find(subscriptionType);
				if (itor != mSubscriptionList.end())
				{
					ProcessorContactThin  &contactThin = *(itor->second);
					if (std::find(contactThin.begin(), contactThin.end(), processor) == contactThin.end())
					{
						contactThin.push_back(processor);
					}
				}
				else
				{
					THROW_EXCEPTION(SimpleException, "failed to subscribe to unknown event type, may forgot to register custom event type");
				}
			}
		}
	}

	// force subscribe for common event
	ProcessorContactThin  &contactThin  = *(mSubscriptionList[EVENT_COMMON]);
	if (std::find(contactThin.begin(), contactThin.end(), processor) == contactThin.end())
	{
		contactThin.push_back(processor);
	}
	
}

void HV::EventCenter::detachProcessor(EventProcessor *processor)
{
	if (NULL == processor)
	{
		THROW_EXCEPTION(SimpleException, "processor pointer should not be NULL");
	}

	for (SubscriptionList::iterator itor = mSubscriptionList.begin(); itor != mSubscriptionList.end(); itor++)
	{
		ProcessorContactThin  &contactThin = *(itor->second);
		ProcessorContactThin::iterator subItor = std::find(contactThin.begin(), contactThin.end(), processor);
		if (subItor != contactThin.end())
		{
			contactThin.erase(std::find(contactThin.begin(), contactThin.end(), processor));
		}
	}
}

void HV::EventCenter::publishEvent()
{
	if (!mEventPool.empty())
	{
		// publish event pool
		for (EventPool::size_type i = 0; i < mEventPool.size(); i++)
		{
			PEvent e = mEventPool[i];
			SubscriptionList::iterator itor = mSubscriptionList.find(e->type);
			if (itor != mSubscriptionList.end())
			{
				ProcessorContactThin  &contactThin = *(itor->second);
				for (ProcessorContactThin::iterator subItor = contactThin.begin(); subItor != contactThin.end(); subItor++)
				{
					if (e->swallowed)
					{
						break;
					}
					else
					{
						(*subItor)->_handleEvent(e);
					}
				}
			}
		}

		// done, now clear it.
		clearEventPool();
	}
}

void HV::EventCenter::registerCustomEventType(EventType type)
{
	if (mSubscriptionList.find(type) != mSubscriptionList.end())
	{
		mSubscriptionList[type] = new ProcessorContactThin();
	}
	else
	{
		THROW_EXCEPTION(SimpleException, "duplicate event type, try to use EVENT_CUSTOM_EVENT_ANCHOR as the anchor of your custom event type id");
	}
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender)
{
	return createStdEvent(type, sender, UINT64(0));
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, INT64 arg)
{
	Event *e = new Event;
	e->sender = sender;
	e->type = type;
	e->swallowed = false;
	e->arg.i64 = arg;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, UINT64 arg)
{
	Event *e = new Event;
	e->sender = sender;
	e->type = type;
	e->swallowed = false;
	e->arg.ui64 = arg;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, void *arg)
{
	Event *e = new Event;
	e->sender = sender;
	e->type = type;
	e->swallowed = false;
	e->arg.p = (INTPTR)arg;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, DECIMAL arg)
{
	Event *e = new Event;
	e->sender = sender;
	e->type = type;
	e->swallowed = false;
	e->arg.dec = arg;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, INT32 arg0, INT32 arg1)
{
	Event *e = new Event;
	e->sender = sender;
	e->type = type;
	e->swallowed = false;
	e->arg.i32a[0] = arg0;
	e->arg.i32a[1] = arg1;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, UINT32 arg0, UINT32 arg1)
{
	Event *e = new Event;
	e->sender = sender;
	e->type = type;
	e->swallowed = false;
	e->arg.ui32a[0] = arg0;
	e->arg.ui32a[1] = arg1;
	return e;
}

HV::PEvent HV::EventCenter::createStdEvent(EventType type, void *sender, const char *arg)
{
	Event *e = new Event;
	e->sender = sender;
	e->type = type;
	e->swallowed = false;
	memset(e->arg.str, 0, 8);
	memcpy(e->arg.str, arg, 7);
	return e;
}

void HV::EventCenter::clearEventPool()
{
	for (EventPool::iterator itor = mEventPool.begin(); itor != mEventPool.end(); itor++)
	{
		SAFE_DELETE(*itor);
	}
	mEventPool.clear();
}
