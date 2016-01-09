/************************************************************************/
/* Logic Sub Scene 1
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#ifndef __SUB_SCENE_1_H__
#define __SUB_SCENE_1_H__

#include "Framework/Scene.h"

struct D3D11InputLayout;
struct ID3D11Buffer;
struct ID3DX11Effect;

class SubScene1 : public HV::Scene
{
public:
	SubScene1() : mWireframeMode(false) {}
	virtual ~SubScene1() {}

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


#endif // __SUB_SCENE_1_H__

// EOF