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
	MainScene() : mWireframeMode(false) {}
	virtual ~MainScene() {}

	virtual void				update(DECIMAL deltaTime);
	virtual void				render(HV::Renderer *renderer);

	virtual void				onEnter();
	virtual void				onExit();

private:
	void						buildGeometryBuffers();
	void						buildFX();
	void						buildVertexInputLayout();

	bool						mWireframeMode;
};


#endif // __MAIN_SCENE_H__

// EOF