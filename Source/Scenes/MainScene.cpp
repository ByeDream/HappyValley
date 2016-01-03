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
#include "Framework/EventDriver/EventCenter.h"

// TODO : remove
#include "d3dx11effect.h"

using namespace HV;

// test code
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;
ID3D11Buffer*           g_pIndexBuffer = NULL;
ID3DX11Effect *			g_effect;

XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

void MainScene::update(DECIMAL deltaTime)
{
	DECIMAL time = mDirector->getApp()->getTimer()->getSuspendableTotalTimeInSecond();

	g_World = XMMatrixRotationY(time);

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1280 / (FLOAT)720, 0.01f, 100.0f);

// 	PEvent e = EventCenter::getInstance()->createStdEvent(EVENT_SWITCH_TO_SCENE, this, SID_SubScene0, 0);
// 	EventCenter::getInstance()->reportEvent(e);
//	mDirector->switchToScene(SID_SubScene0);

}

void MainScene::render(HV::Renderer *renderer)
{
	ID3D11DeviceContext *renderConext = renderer->getRenderContext();
	// for one object
	{
		renderConext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderConext->IASetInputLayout(g_pVertexLayout);
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		renderConext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
		renderConext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// update constant buffers
		ID3DX11EffectMatrixVariable *constantWVP = g_effect->GetConstantBufferByName("gWorldViewProj")->AsMatrix();
		XMMATRIX WVP = g_World * g_View * g_Projection;
		constantWVP->SetMatrix(reinterpret_cast<float*>(&WVP));

		ID3DX11EffectTechnique *tech = g_effect->GetTechniqueByName("SolidTech");
		//ID3DX11EffectTechnique *tech = g_effect->GetTechniqueByName("WireframeTech");
		D3DX11_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);
		for (INDEX_T i = 0; i < techDesc.Passes; i++)
		{
			tech->GetPassByIndex(i)->Apply(0, renderConext);
			renderConext->DrawIndexed(36, 0, 0); // 36 vertices needed for 12 triangles in a triangle list
		}
	}
}

void MainScene::onEnter()
{
	g_World = XMMatrixIdentity();
	g_View = XMMatrixIdentity();
	g_Projection = XMMatrixIdentity();

	buildGeometryBuffers();
	buildFX();
	buildVertexInputLayout();
}

void MainScene::onExit()
{
	SAFE_RELEASE(g_pVertexLayout);
	SAFE_RELEASE(g_effect);
	SAFE_RELEASE(g_pVertexBuffer);
	SAFE_RELEASE(g_pIndexBuffer);
}

void MainScene::buildGeometryBuffers()
{
	// Create the vertex buffer
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
	SIZE_T numVertices = SIZE_OF_ARRAY(vertices);

	D3D11_BUFFER_DESC bd;
	OBJ_MEM_CLEAR(bd);
	bd.Usage = D3D11_USAGE_IMMUTABLE; // D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	OBJ_MEM_CLEAR(InitData);
	InitData.pSysMem = vertices;

	HRESULT hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "failed to create vertex buffer");
	}

	// Create the index buffer
 	UINT16 indices[] =
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
	SIZE_T numIndices = SIZE_OF_ARRAY(indices);

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(UINT16) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = indices;

	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "failed to create index buffer");
	}
}

void MainScene::buildFX()
{
#if 1
	// read compiled fx file
	std::ifstream fin("SampleEffect.fxo", std::ios::binary);
	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledFX(size);
	fin.read(&compiledFX[0], size);
	fin.close();

	HRESULT hr = D3DX11CreateEffectFromMemory(	&compiledFX[0], 
												size,
												0, 
												mDirector->getApp()->getRenderer()->getRenderDevice(),
												&g_effect);

	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "failed to create effect");
	}
#else
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#	if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#	endif

		ID3DBlob *compiledShader = NULL;
		ID3DBlob *compilationMsgs = NULL;
		HRESULT hr = D3DX11CompileFromFile(	"SampleEffect.fx", 
											NULL,
											NULL, 
											NULL,  // function, set as NULL for fx file
											"fx_5_0",
											shaderFlags,
											0, 
											NULL, 
											&compiledShader,
											&compilationMsgs,
											NULL);
		if (compilationMsgs != NULL)
		{
			char msgStr[1024] = {0};
			sprintf_s(msgStr, 1023, "%s", (char*)compilationMsgs->GetBufferPointer());
			SAFE_RELEASE(compilationMsgs);
			THROW_EXCEPTION(SimpleException, msgStr);
		}
		if (FAILED(hr))
		{
			THROW_EXCEPTION(SimpleException, "some bad things happend while shader compiling");
		}

		hr = D3DX11CreateEffectFromMemory(	compiledShader->GetBufferPointer(),
											compiledShader->GetBufferSize(),
											0,
											mDirector->getApp()->getRenderer()->getRenderDevice(),
											&g_effect);

		if (FAILED(hr))
		{
			THROW_EXCEPTION(SimpleException, "failed to create effect");
		}

		SAFE_RELEASE(compiledShader);
#endif
}

void MainScene::buildVertexInputLayout()
{
	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	D3DX11_PASS_DESC passDesc;
	HRESULT hr = g_effect->GetTechniqueByName("SolidTech")->GetPassByIndex(0)->GetDesc(&passDesc);
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "get effect pass description failed");
	}

	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateInputLayout(
		layout,
		numElements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&g_pVertexLayout);
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "create input layout failed");
	}
}

HV::FACTORY::REGISTER_AUTO_WORKER(MainScene, SID_MainScene, HV::Director::sGetSceneFactory())
// EOF
