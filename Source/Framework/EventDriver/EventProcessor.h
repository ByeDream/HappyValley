/************************************************************************/
/* The Base Class Of Event Processor
/*
/* ByeDream
/* Dec-28-2015
/************************************************************************/
#ifndef __EVENT_PROCESSOR_H__
#define __EVENT_PROCESSOR_H__

#include "Framework/EventDriver/Event.h"

namespace HV
{
	// return true if you want to swallow the event
	typedef bool(*EventHandleFunc)(CPEvent e, void *pOwner);

	class EventProcessor
	{
		friend class EventCenter;
	public:
		EventProcessor();
		virtual ~EventProcessor() {}

		virtual void			setOwner(void * pOwner, EventHandleFunc pFunc) { mOwner = pOwner; mFunc = pFunc; }
		// return true if you want to swallow the event
		virtual bool			handleEvent(CPEvent e) { bool swallowEvent = false; return swallowEvent; }

		virtual void			registerToEventCenter(EventType *subscriptionList, SIZE_T sizeOfList);
		virtual void			unregisterFromEventCenter();

	protected:
		EventHandleFunc			mFunc;
		void *					mOwner;
	private:
		void					_handleEvent(PEvent e);
	};
}

#endif // !__EVENT_PROCESSOR_H__