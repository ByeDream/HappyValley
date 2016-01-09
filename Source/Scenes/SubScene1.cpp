/************************************************************************/
/* Logic Scene 0
/*
/* ByeDream
/* Dec-24-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "SubScene1.h"

#include "ScenesHeader.h"
#include "Framework/Modules/Director/Director.h"

#include "Framework/Math/Math.h"
#include "Framework/Application.h"
#include "Framework/Modules/Timer/Timer.h"
#include "Framework/Modules/Rendering/Renderer.h"
#include "Framework/EventDriver/EventCenter.h"

// TODO : remove
#include "d3dx11effect.h"

namespace {
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	XMMATRIX                g_World[5];
	XMMATRIX                g_WorldSphere[3];
	XMMATRIX                g_View;
	XMMATRIX                g_Projection;

	ID3D11InputLayout*      g_pVertexLayout;
	ID3D11Buffer*           g_pVertexBuffer;
	ID3D11Buffer*           g_pIndexBuffer;
	ID3DX11Effect *			g_effect;

	UINT32					g_IndexOffset[5];
	UINT32					g_IndexCount[5];
	UINT32					g_VertexOffset[5];
}

using namespace HV;

void SubScene1::update(DECIMAL deltaTime)
{
	DECIMAL time = mDirector->getApp()->getTimer()->getSuspendableTotalTimeInSecond();

	mWireframeMode = ((UINT32)time % 6) < 3;

	XMMATRIX s = XMMatrixScaling(1.2f, 1.2f, 1.2f);
	XMMATRIX t = XMMatrixTranslation(0.5f, 0.6f, 1.5f);
	g_World[0] = s * t;

	s = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	t = XMMatrixTranslation(-0.5f, 1.0f, 0.5f);
	g_World[1] = s * t;

	XMMATRIX r = XMMatrixRotationX(XM_PIDIV2);
	t = XMMatrixTranslation(0.0f, 5.0f, -2.0f);
	g_World[2] = r * t;

	s = XMMatrixScaling(1.4f, 1.4f, 1.4f);
	t = XMMatrixTranslation(-1.0f, 1.0f, 2.5f);
	g_WorldSphere[0] = s * t;

	s = XMMatrixScaling(0.4f, 0.4f, 0.4f);
	XMMATRIX t1 = XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	r = XMMatrixRotationY(time);
	t = XMMatrixTranslation(0.4f, 2.6f, 0.2f);
	g_WorldSphere[1] = s * t1 * r * t;
	g_WorldSphere[2] = t;

// 	XMMATRIX =
// 		g_World[i] = XMMatrixTranslation(0.5f, 0.7f, 1.5f);
// 
// 	g_World[i] = XMMatrixTranslation(-0.5f, 2.0f, 1.3f);
// 
// 
// 	g_WorldSphere[0] = XMMatrixTranslation(-0.5f, 2.0f, 1.3f);
// 	g_WorldSphere[1] = XMMatrixTranslation(-0.5f, 2.0f, 1.3f);
// 	g_WorldSphere[2] = XMMatrixTranslation(-0.5f, 2.0f, 1.3f);

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(10.0f * sinf(time * 0.2f), 3.0f, 10.0f * cosf(time * 0.2f), 1.0f);
	//XMVECTOR Eye = XMVectorSet(0.0f, 5.0f, 10.0f, 1.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1280 / (FLOAT)720, 0.01f, 10000.0f);

	// 	PEvent e = EventCenter::getInstance()->createStdEvent(EVENT_SWITCH_TO_SCENE, this, SID_SubScene0, 0);
	// 	EventCenter::getInstance()->reportEvent(e);
	//	mDirector->switchToScene(SID_SubScene0);

}

void SubScene1::render(HV::Renderer *renderer)
{
	ID3D11DeviceContext *renderConext = renderer->getRenderContext();

	renderConext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderConext->IASetInputLayout(g_pVertexLayout);
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	renderConext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	renderConext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	ID3DX11EffectMatrixVariable *constantWVP = g_effect->GetVariableByName("gWorldViewProj")->AsMatrix();

	for (INDEX_T i = 0; i < 4; i++)
	{
		// update constant buffers
		XMMATRIX WVP = g_World[i] * g_View * g_Projection;
		constantWVP->SetMatrix(reinterpret_cast<float*>(&WVP));

		ID3DX11EffectTechnique *tech = mWireframeMode ? g_effect->GetTechniqueByName("WireframeTech") : g_effect->GetTechniqueByName("SolidTech");
		D3DX11_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);
		for (INDEX_T j = 0; j < techDesc.Passes; j++)
		{
			tech->GetPassByIndex(j)->Apply(0, renderConext);
			renderConext->DrawIndexed(g_IndexCount[i], g_IndexOffset[i], g_VertexOffset[i]);
		}
	}

	for (INDEX_T i = 0; i < 3; i++)
	{
		// update constant buffers
		XMMATRIX WVP = g_WorldSphere[i] * g_View * g_Projection;
		constantWVP->SetMatrix(reinterpret_cast<float*>(&WVP));

		ID3DX11EffectTechnique *tech = mWireframeMode ? g_effect->GetTechniqueByName("WireframeTech") : g_effect->GetTechniqueByName("SolidTech");
		D3DX11_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);
		for (INDEX_T j = 0; j < techDesc.Passes; j++)
		{
			tech->GetPassByIndex(j)->Apply(0, renderConext);
			renderConext->DrawIndexed(g_IndexCount[4], g_IndexOffset[4], g_VertexOffset[4]);
		}
	}
}

void SubScene1::onEnter()
{
	for (INDEX_T i = 0; i < 5; i++)
	{
		g_World[i] = XMMatrixIdentity();
	}

	for (INDEX_T i = 0; i < 3; i++)
	{
		g_WorldSphere[i] = XMMatrixIdentity();
	}

	
	g_View = XMMatrixIdentity();
	g_Projection = XMMatrixIdentity();

	buildGeometryBuffers();
	buildFX();
	buildVertexInputLayout();
}

void SubScene1::onExit()
{
	SAFE_RELEASE(g_pVertexLayout);
	SAFE_RELEASE(g_effect);
	SAFE_RELEASE(g_pVertexBuffer);
	SAFE_RELEASE(g_pIndexBuffer);
}

void SubScene1::buildGeometryBuffers()
{
	std::vector<SimpleVertex> vertices;
	std::vector<UINT16> indices;
	UINT32 totalVertexCount = 0;
	UINT32 totalIndexCount = 0;
	for (INDEX_T i = 0; i < 5; i++)
	{
		GeometryGenerator::MeshData meshData;
		XMFLOAT4 meshColor;
		if (0 == i)
		{
			GeometryGenerator::createBox(1.0f, 1.0f, 1.0f, meshData);
			meshColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (1 == i)
		{
			GeometryGenerator::createCylinder(0.5f, 0.5f, 1.0f, 20, 1, meshData);
			meshColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (2 == i)
		{
			GeometryGenerator::createGrid(10.0f, 10.0f, 20, 20, meshData);
			meshColor = XMFLOAT4(0.2f, 0.7f, 0.5f, 1.0f);

		}
		else if (3 == i)
		{
			GeometryGenerator::createGrid(10.0f, 10.0f, 20, 20, meshData);
			meshColor = XMFLOAT4(0.2f, 0.5f, 0.7f, 1.0f);
		}
		else
		{
			GeometryGenerator::createGeosphere(0.5f, 3, meshData);
			meshColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		}

		for (GeometryGenerator::VertexList::const_iterator itor = meshData.Vertices.begin(); itor != meshData.Vertices.end(); itor++)
		{
			SimpleVertex v;
			v.Pos = itor->Position;
			v.Color = meshColor;
			vertices.push_back(v);
		}
		indices.insert(indices.end(), meshData.Indices.begin(), meshData.Indices.end());

		g_IndexOffset[i] = totalIndexCount;
		g_IndexCount[i] = meshData.Indices.size();
		g_VertexOffset[i] = totalVertexCount;

		totalVertexCount += meshData.Vertices.size();
		totalIndexCount += meshData.Indices.size();
	}

	// Create the vertex buffer
	D3D11_BUFFER_DESC bd;
	OBJ_MEM_CLEAR(bd);
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(SimpleVertex) * totalVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	OBJ_MEM_CLEAR(InitData);
	InitData.pSysMem = &vertices[0];

	HRESULT hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "failed to create vertex buffer");
	}

	// Create the index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(UINT16) * totalIndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = &indices[0];

	hr = mDirector->getApp()->getRenderer()->getRenderDevice()->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "failed to create index buffer");
	}
}

void SubScene1::buildFX()
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

	HRESULT hr = D3DX11CreateEffectFromMemory(&compiledFX[0],
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
	HRESULT hr = D3DX11CompileFromFile("SampleEffect.fx",
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
		char msgStr[1024] = { 0 };
		sprintf_s(msgStr, 1023, "%s", (char*)compilationMsgs->GetBufferPointer());
		SAFE_RELEASE(compilationMsgs);
		THROW_EXCEPTION(SimpleException, msgStr);
	}
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "some bad things happend while shader compiling");
	}

	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(),
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

void SubScene1::buildVertexInputLayout()
{
	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	SIZE_T numElements = SIZE_OF_ARRAY(layout);

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

HV::FACTORY::REGISTER_AUTO_WORKER(SubScene1, SID_SubScene0, HV::Director::sGetSceneFactory())
// EOF
