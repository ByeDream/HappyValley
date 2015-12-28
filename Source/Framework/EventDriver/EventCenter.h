/************************************************************************/
/* The Event Manager
/*
/* ByeDream
/* Dec-28-2015
/************************************************************************/
#ifndef __EVENT_CENTER_H__
#define __EVENT_CENTER_H__

#include "Framework/EventDriver/Event.h"
#include "Framework/DesignPattern/Singleton.h"

namespace HV
{
	class EventProcessor;
	class EventCenter : public Singleton<EventCenter>
	{
		friend class Singleton<EventCenter>;
		typedef std::list<EventProcessor *> ProcessorContactThin;
		typedef std::map<UINT32, ProcessorContactThin> SubscriptionList;
		typedef std::vector<PEvent> EventPool;
	public:
		virtual void			initialize();
		virtual void			finitialize();

		PEvent					createStdEvent(EventType type, void *sender, INT64 arg);
		PEvent					createStdEvent(EventType type, void *sender, UINT64 arg);
		PEvent					createStdEvent(EventType type, void *sender, INTPTR arg);
		PEvent					createStdEvent(EventType type, void *sender, DECIMAL arg);
		PEvent					createStdEvent(EventType type, void *sender, INT32 arg0, INT32 arg1);
		PEvent					createStdEvent(EventType type, void *sender, UINT32 arg0, UINT32 arg1);
		PEvent					createStdEvent(EventType type, void *sender, const char *arg);

		void					reportEvent(PEvent e);
		void					attachProcessor(EventProcessor *processor, EventType *subscriptionList, SIZE_T sizeOfList);
		void					detachProcessor(EventProcessor *processor);
		void					publishEvent();
		void					registerCustomEventType(EventType type);

	private:
		EventCenter();
		~EventCenter();

		SubscriptionList		mSubscriptionList;
		EventPool				mEventPool;
	};

	// TODO : double pool, lock and multiple thread safe.
}

#endif // !__EVENT_CENTER_H__
