/************************************************************************/
/* The General Interface Of Terminal
/*
/* ByeDream
/* Dec-26-2015
/************************************************************************/
#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "Framework/Modules/Module.h"

namespace HV
{
	class Terminal : public Module
	{
	public:
		Terminal(Application *app);
		virtual ~Terminal() {}

		virtual void				initialize();
		virtual void				finitialize();

		virtual void				show() {}
	protected:
	};
}

#endif // !__TIMER_H__
