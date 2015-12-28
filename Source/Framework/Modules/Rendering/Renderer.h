/************************************************************************/
/* The General Interface Of Renderer
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Framework/Modules/Module.h"

namespace HV
{
	class Renderer : public Module
	{
	public:
		Renderer(Application *app) : Module(app) {}
		virtual ~Renderer() {}

		virtual void				beginScene() = 0;
		virtual void				endScene() = 0;
		virtual void				clear() = 0;
		virtual void				present() = 0;

		//////////////////////////////
		// Only for DXD11
		// TODO remove
		virtual ID3D11Device *				getRenderDevice() const { return NULL; }
		virtual ID3D11DeviceContext *		getRenderContext() const { return NULL; }
		virtual IDXGISwapChain *			getSwapChain() const { return NULL; }
		virtual ID3D11RenderTargetView *	getRenderTarget() const { return NULL; }
		virtual ID3D11DepthStencilView *	getDepthTarget() const { return NULL; }
		//////////////////////////////
	};
}

#endif // !__RENDERER_H__
