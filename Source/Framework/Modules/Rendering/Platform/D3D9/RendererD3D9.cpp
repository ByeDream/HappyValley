/************************************************************************/
/* The Renderer Base On D3D 9
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "RendererD3D9.h"

#include "Framework/Application.h"
#include "Framework/Modules/Terminal/Platform/Windows/TerminalWin.h"

HV::RendererD3D9::RendererD3D9(Application *app)
	: Renderer(app)
	, mD3D9(NULL)
	, mD3DDevice(NULL)
{

}

HV::RendererD3D9::~RendererD3D9()
{

}

void HV::RendererD3D9::initialize()
{
	TerminalWin * terminal = dynamic_cast<TerminalWin *>(mApp->getTerminal());
	if (!terminal)
	{
		THROW_EXCEPTION(SimpleException, "initialize renderer d3d9 failed");
	}
	HWND wnd = terminal->getWindow();

	RECT rc;
	GetClientRect(wnd, &rc);

	// init D3D
	mD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;
	mD3D9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,    // main display card
		D3DDEVTYPE_HAL,
		&caps
		);
	// ...

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = rc.right - rc.left;
	d3dpp.BackBufferHeight = rc.bottom - rc.top;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		// TODO : msaa
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = wnd;
	d3dpp.Windowed = TRUE;								// windowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = mD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		wnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&mD3DDevice
		);

	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "create D3D 9 device failed");
	}
}

void HV::RendererD3D9::finitialize()
{
	SAFE_RELEASE(mD3DDevice);
	SAFE_RELEASE(mD3D9);
}

void HV::RendererD3D9::clear()
{
	if (mD3DDevice)
	{
		// Clear the back buffer to a blue color
		HRESULT hr = mD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(200, 200, 200), 1.0f, 0);
		if (FAILED(hr))
		{
			THROW_EXCEPTION(SimpleException, "clear back buffer failed");
		}
	}
}

void HV::RendererD3D9::present()
{
	if (mD3DDevice)
	{
		// Present the back buffer contents to the display
		HRESULT hr = mD3DDevice->Present(NULL, NULL, NULL, NULL);
		if (FAILED(hr))
		{
			THROW_EXCEPTION(SimpleException, "present back buffer failed");
		}
	}
}

void HV::RendererD3D9::beginScene()
{
	if (mD3DDevice)
	{
		HRESULT hr = mD3DDevice->BeginScene();
		if (FAILED(hr))
		{
			THROW_EXCEPTION(SimpleException, "RendererD3D9 begin scene failed");
		}
	}
}

void HV::RendererD3D9::endScene()
{
	if (mD3DDevice)
	{
		HRESULT hr = mD3DDevice->EndScene();
		if (FAILED(hr))
		{
			THROW_EXCEPTION(SimpleException, "RendererD3D9 end scene failed");
		}
	}
}

// EOF