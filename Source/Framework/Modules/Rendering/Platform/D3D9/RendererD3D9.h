/************************************************************************/
/* The Renderer Base On D3D 9
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#ifndef __RENDERER_D3D9_H__
#define __RENDERER_D3D9_H__

#include "Framework/Modules/Rendering/Renderer.h"

namespace HV
{
	class RendererD3D9 : public Renderer
	{
	public:
		RendererD3D9(Application *app);
		virtual ~RendererD3D9();

		virtual void				initialize();
		virtual void				finitialize();

		virtual void				beginScene();
		virtual void				endScene();
		virtual void				clear();
		virtual void				present();
	private:
		IDirect3D9 *				mD3D9;
		IDirect3DDevice9 *			mD3DDevice;
	};
}

#endif // !__RENDERER_D3D9_H__

// EOF