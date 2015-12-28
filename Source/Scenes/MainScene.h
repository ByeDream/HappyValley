/************************************************************************/
/* Logic Scene 0
/*
/* ByeDream
/* Dec-24-2015
/************************************************************************/
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "Framework/Scene.h"

class MainScene : public HV::Scene
{
public:
	virtual void				update(DECIMAL deltaTime);
	virtual void				render(HV::Renderer *renderer);

	virtual void				onEnter();
	virtual void				onExit();
};


#endif // __MAIN_SCENE_H__

// EOF