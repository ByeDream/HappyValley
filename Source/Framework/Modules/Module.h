/************************************************************************/
/* The General Interface Of Application Modules
/*
/* ByeDream
/* Dec-24-2015
/************************************************************************/
#ifndef __MODULE_H__
#define __MODULE_H__

namespace HV
{
	class Application;

	class Module
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