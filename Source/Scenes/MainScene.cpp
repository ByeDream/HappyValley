/************************************************************************/
/* Logic Scene 0
/*
/* ByeDream
/* Dec-24-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "MainScene.h"

#include "ScenesHeader.h"
#include "Framework/Modules/Director/Director.h"

#include "Framework/Math/Math.h"
#include "Framework/Application.h"
#include "Framework/Modules/Timer/Timer.h"
#include "Framework/Modules/Rendering/Renderer.h"

// test code
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;
ID3D11Buffer*           g_pIndexBuffer = NULL;
ID3D11Buffer*           g_pConstantBuffer = NULL;

XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}



void MainScene::update(DECIMAL deltaTime)
{
	DECIMAL time = mDirector->getApp()->getTimer()->getSuspendableTotalTimeInSecond();

	g_World = XMMatrixRotationY(time);

	// Initialize the world matrix
	//g_World = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1280 / (FLOAT)720, 0.01f, 100.0f);
}

void MainScene::render(HV::Renderer *renderer)
{

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);

	renderer->getRenderContext()->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);


	renderer->getRenderContext()->VSSetShader(g_pVertexShader, NULL, 0);
	renderer->getRenderContext()->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	renderer->getRenderContext()->PSSetShader(g_pPixelShader, NULL, 0);
	// no pixel constant for this test case

	renderer->getRenderContext()->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list


	// mDirector->switchToScene(SID_SubScene0);
}

void MainScene::onEnter()
{
	// Create the vertex shader
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr = CompileShaderFromFile("testShader.fx", "VS", "vs_4_0", &pVSBlob);
//	if (FAILED(hr))

	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
//	if (FAILED(hr))

	// Create the input layout
 	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
 	UINT numElements = ARRAYSIZE(layout);

	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
 		pVSBlob->GetBufferSize(), &g_pVertexLayout);

 	pVSBlob->Release();
// 	if (FAILED(hr))

	mDirector->getApp()->getRenderer()->getRenderContext()->IASetInputLayout(g_pVertexLayout);

	// Create the pixel shader
 	ID3DBlob* pPSBlob = NULL;
 	hr = CompileShaderFromFile("testShader.fx", "PS", "ps_4_0", &pPSBlob);
 	if (FAILED(hr))
 	
	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
 	pPSBlob->Release();
// 	if (FAILED(hr))
 
 	// Create vertex buffer
 	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};

 	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
//	if (FAILED(hr))

 	UINT stride = sizeof(SimpleVertex);
 	UINT offset = 0;
	mDirector->getApp()->getRenderer()->getRenderContext()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
 
 	// Create index buffer
 	WORD indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		0, 5, 4,
		1, 5, 0,

		3, 4, 7,
		0, 4, 3,

		1, 6, 5,
		2, 6, 1,

		2, 7, 6,
		3, 7, 2,

		6, 4, 5,
		7, 4, 6,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
//	if (FAILED(hr))

	mDirector->getApp()->getRenderer()->getRenderContext()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
 
 	// Set primitive topology
	mDirector->getApp()->getRenderer()->getRenderContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
 	// Create the constant buffer
 	bd.Usage = D3D11_USAGE_DEFAULT;
 	bd.ByteWidth = sizeof(ConstantBuffer);
 	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 	bd.CPUAccessFlags = 0;
	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
// 	if (FAILED(hr))

}

void MainScene::onExit()
{
// 	if (g_pConstantBuffer) g_pConstantBuffer->Release();
// 	if (g_pVertexBuffer) g_pVertexBuffer->Release();
// 	if (g_pIndexBuffer) g_pIndexBuffer->Release();
// 	if (g_pVertexLayout) g_pVertexLayout->Release();
// 	if (g_pVertexShader) g_pVertexShader->Release();
// 	if (g_pPixelShader) g_pPixelShader->Release();
}

HV::FACTORY::REGISTER_AUTO_WORKER(MainScene, SID_MainScene, HV::Director::sGetSceneFactory())
// EOF
