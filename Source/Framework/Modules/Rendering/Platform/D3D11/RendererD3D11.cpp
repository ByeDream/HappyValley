/************************************************************************/
/* The Renderer Base On D3D 11
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "RendererD3D11.h"

#include "Framework/Application.h"
#include "Framework/Modules/Terminal/Platform/Windows/TerminalWin.h"

HV::RendererD3D11::RendererD3D11(Application *app)
	: Renderer(app)
	, mD3DDevice(NULL)
	, mImmediateContext(NULL)
	, mSwapChain(NULL)
	, mRenderTargetView(NULL)
	, mDepthStencilView(NULL)
{

}

HV::RendererD3D11::~RendererD3D11()
{

}

void HV::RendererD3D11::initialize()
{
	TerminalWin * terminal = dynamic_cast<TerminalWin *>(mApp->getTerminal());
	if (!terminal)
	{
		THROW_EXCEPTION(SimpleException, "initialize renderer d3d11 failed");
	}
	HWND wnd = terminal->getWindow();

	// create device
	RECT rc;
	GetClientRect(wnd, &rc);

	UINT32 createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT32 numDriverTypes = SIZE_OF_ARRAY(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT32 numFeatureLevels = SIZE_OF_ARRAY(featureLevels);

	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevel;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		hr = D3D11CreateDevice(
			NULL,								// default adapter
			driverTypes[driverTypeIndex],
			NULL,								// no software device todo: maybe a custom soft device self-made, should be fun
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&mD3DDevice,
			&featureLevel,
			&mImmediateContext);

		if (SUCCEEDED(hr) && featureLevel == D3D_FEATURE_LEVEL_11_0)
			break;
	}
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "create D3D 11 device failed");
	}

	// check level of msaa
	UINT32 mMassSampleCount = 4;
	UINT32 mMsaaQuality;
	mD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mMassSampleCount, &mMsaaQuality);

	// create swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rc.right - rc.left;
	sd.BufferDesc.Height = rc.bottom - rc.top;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.OutputWindow = wnd;
	if (mMsaaQuality > 0)
	{
		sd.SampleDesc.Count = mMassSampleCount;
		sd.SampleDesc.Quality = mMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.Windowed = TRUE;							// windowed
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice *dxgiDevice = NULL;
	mD3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	IDXGIAdapter *dxgiAdapter = NULL;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
	IDXGIFactory *dxgiFactory = NULL;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
	dxgiFactory->CreateSwapChain(mD3DDevice, &sd, &mSwapChain);

	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	// create render target
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	mD3DDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	SAFE_RELEASE(backBuffer);

	// create depth-stencil target
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = rc.right - rc.left;
	depthStencilDesc.Height = rc.bottom - rc.top;
	depthStencilDesc.MipLevels = 1;					// no need for depth-stencil target
	depthStencilDesc.ArraySize = 1;					// no need for depth-stencil target
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (mMsaaQuality > 0)
	{
		depthStencilDesc.SampleDesc.Count = mMassSampleCount;
		depthStencilDesc.SampleDesc.Quality = mMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D *mDepthStencilBuffer;
	mD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// set view viewport (By modifying the viewport, we can draw the 3D scene into a subrectangle)
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(rc.right - rc.left);
	vp.Height = static_cast<float>(rc.bottom - rc.top);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	mImmediateContext->RSSetViewports(1, &vp);
}

void HV::RendererD3D11::finitialize()
{
	if (mImmediateContext) 
		mImmediateContext->ClearState();

	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mSwapChain);
	SAFE_RELEASE(mImmediateContext);
	SAFE_RELEASE(mD3DDevice);
}

void HV::RendererD3D11::clear()
{
	// Clear the back buffer
	if (mImmediateContext)
	{
		float ClearColor[4] = { 0.75f, 0.75f, 0.75f, 1.0f };
		mImmediateContext->ClearRenderTargetView(mRenderTargetView, ClearColor);

		mImmediateContext->ClearDepthStencilView(mDepthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void HV::RendererD3D11::present()
{
	if (mSwapChain)
	{
		// Present the back buffer contents to the display
		HRESULT hr = mSwapChain->Present(NULL, NULL);
		if (FAILED(hr))
		{
			THROW_EXCEPTION(SimpleException, "present back buffer failed");
		}
	}
}

void HV::RendererD3D11::beginScene()
{

}

void HV::RendererD3D11::endScene()
{

}

// EOF