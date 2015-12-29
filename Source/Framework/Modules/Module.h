/************************************************************************/
/* The General Interface Of Application Modules
/*
/* ByeDream
/* Dec-24-2015
/************************************************************************/
#ifndef __MODULE_H__
#define __MODULE_H__

#include "Framework/EventDriver/EventProcessor.h"

namespace HV
{
	class Application;

	class Module : public EventProcessor
	{
	public:
		Module(Application *app) : mApp(app) {}
		virtual ~Module() {}

		virtual void				initialize() = 0;
		virtual void				finitialize() = 0;

		virtual Application *		getApp() { return mApp; }
	protected:
		Application *mApp;
	};
}

#endif // !__MODULE_H__