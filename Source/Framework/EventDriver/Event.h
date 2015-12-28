/************************************************************************/
/* The Event Class And Standard Event Type
/*
/* ByeDream
/* Dec-28-2015
/************************************************************************/
#ifndef __EVENT_H__
#define __EVENT_H__

namespace HV
{
	enum StdEventType
	{
		EVENT_COMMON = 0,

		EVENT_SWITCH_TO_SCENE,
		EVENT_SHUTDOWN_APP,

		EVENT_COUNT,
		EVENT_CUSTOM_EVENT_ANCHOR = EVENT_COUNT,
	};

	typedef UINT32 EventType;
	typedef struct _Event_
	{
		union _EventArg
		{
			INT64			i64;
			UINT64			ui64;
			INT32			i32a[2];
			UINT32			ui32a[2];
			INT8			str[8];
			INTPTR			p;
			DECIMAL			dec;
		}					arg;

		void *				sender;
		EventType			type;
		bool				swallowed;
	} Event, *PEvent;
	typedef const PEvent CPEvent;
}


#endif // !__EVENT_H__