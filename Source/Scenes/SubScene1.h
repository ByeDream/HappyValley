/************************************************************************/
/* Logic Sub Scene 1
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#ifndef __SUB_SCENE_1_H__
#define __SUB_SCENE_1_H__

#include "Framework/Scene.h"

class SubScene1 : public HV::Scene
{
public:
	virtual void				update(DECIMAL deltaTime);
	virtual void				render(HV::Renderer *renderer);
};


#endif // __SUB_SCENE_1_H__

// EOF