/************************************************************************/
/* The Director That Is Responsible For Scene Management
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include "Framework/Modules/Module.h"

namespace HV
{
	class Scene;
	class Renderer;
	namespace FACTORY
	{
		class Factory;
	}

	class Director : public Module
	{
	public:
		Director(Application *app);
		virtual ~Director();

		virtual void				initialize();
		virtual void				finitialize();
		virtual void				update(DECIMAL deltaTime);
		virtual void				render(Renderer *renderer);

		virtual void				switchToScene(INDEX_T id);
		virtual Scene *				getCurrentActiveScene() { return mCurrentActiveScene; }

		static FACTORY::Factory *	sGetSceneFactory();

		virtual bool				handleEvent(CPEvent e);

	protected:
		Scene *						mCurrentActiveScene;
	};
}

#endif // !__DIRECTOR_H__
