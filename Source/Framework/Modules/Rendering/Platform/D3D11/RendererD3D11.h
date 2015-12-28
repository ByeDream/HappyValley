/************************************************************************/
/* The Renderer Base On D3D 11
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#ifndef __RENDERER_D3D11_H__
#define __RENDERER_D3D11_H__

#include "Framework/Modules/Rendering/Renderer.h"

namespace HV
{
	class RendererD3D11 : public Renderer
	{
	public:
		RendererD3D11(Application *app);
		virtual ~RendererD3D11();

		virtual void				initialize();
		virtual void				finitialize();

		virtual void				beginScene();
		virtual void				endScene();
		virtual void				clear();
		virtual void				present();

		//////////////////////////////
		// Only for DXD11
		// TODO remove
		virtual ID3D11Device *				getRenderDevice() const { return mD3DDevice; }
		virtual ID3D11DeviceContext *		getRenderContext() const { return mImmediateContext; }
		virtual IDXGISwapChain *			getSwapChain() const { return mSwapChain; }
		virtual ID3D11RenderTargetView *	getRenderTarget() const { return mRenderTargetView; }
		virtual ID3D11DepthStencilView *	getDepthTarget() const { return mDepthStencilView; }
		//////////////////////////////

	private:
		ID3D11Device *				mD3DDevice;
		ID3D11DeviceContext *		mImmediateContext;
		IDXGISwapChain *			mSwapChain;

		ID3D11RenderTargetView *	mRenderTargetView;
		ID3D11DepthStencilView *	mDepthStencilView;
	};
}

#endif // !__RENDERER_D3D11_H__

// EOF