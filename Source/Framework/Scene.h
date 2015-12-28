/************************************************************************/
/* The General Interface Of Scene
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#ifndef __SCENE_H__
#define __SCENE_H__

#include "DesignPattern/Factory.h"

namespace HV
{
	class Renderer;
	class Director;
	class Scene : public FACTORY::Product
	{
	public:
		Scene() : mDirector(NULL) {}
		virtual ~Scene() {}

		virtual void				bindDirector(Director *director) { mDirector = director; }
		virtual void				update(DECIMAL deltaTime) {}
		virtual void				render(Renderer *renderer) {}

		virtual void				onEnter() {}
		virtual void				onExit() {}
	protected:
		Director *					mDirector;
	};
}

#endif // !__SCENE_H__
