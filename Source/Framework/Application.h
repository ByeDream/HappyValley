/************************************************************************/
/* The General Interface Of Application
/* 
/* ByeDream
/* Dec-20-2015
/************************************************************************/

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Framework/EventDriver/EventProcessor.h"

namespace HV
{
	class Terminal;
	class Renderer;
	class Director;
	class Timer;

	class Application : EventProcessor
	{
	public:
		Application();
		virtual ~Application();
		virtual void			boot();
		virtual void			doFrame();
		virtual void			shutDown();
		virtual bool			isRunning() const { return mIsRunning; }

		inline Terminal *		getTerminal() const { return mTerminal; }
		inline Renderer *		getRenderer() const { return mRenderer; }
		inline Director *		getDirector() const { return mDirector; }
		inline Timer *			getTimer() const { return mTimer; }

		virtual bool			handleEvent(CPEvent e);

	protected:
		virtual void			attachModulues();
		virtual void			detachModulues();

		bool					mIsRunning;

		// modules
		Terminal *				mTerminal;
		Renderer *				mRenderer;
		Director *				mDirector;
		Timer *					mTimer;
	};
}

#endif // !__APPLICATION_H__

// EOF