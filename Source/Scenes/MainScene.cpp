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
namespace {
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	ID3D11InputLayout*      g_pVertexLayout = NULL;
	ID3D11Buffer*           g_pVertexBuffer = NULL;
	SIZE_T					g_numVertices = 0;
	ID3D11Buffer*           g_pIndexBuffer = NULL;
	SIZE_T					g_numIndices = 0;
	ID3DX11Effect *			g_effect;

	XMMATRIX                g_World;
	XMMATRIX                g_View;
	XMMATRIX                g_Projection;
}

void MainScene::update(DECIMAL deltaTime)
{
	DECIMAL time = mDirector->getApp()->getTimer()->getSuspendableTotalTimeInSecond();

	mWireframeMode = ((UINT32)time % 6) < 3;

	g_World = XMMatrixRotationY(time * 0.01f);

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, 150.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1280 / (FLOAT)720, 0.01f, 10000.0f);

	// update dynamic vertex;
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HRESULT hr = mDirector->getApp()->getRenderer()->getRenderContext()->Map(
		g_pVertexBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedData
		);
	if (FAILED(hr))
	{
		THROW_EXCEPTION(SimpleException, "failed to map buffer");
	}
	for (INDEX_T i = 0; i < g_numVertices; i++)
	{
		SimpleVertex *vertices = reinterpret_cast<SimpleVertex*>(mappedData.pData);
		updateDynamicVertex(&vertices[i], time);
	}
	mDirector->getApp()->getRenderer()->getRenderContext()->Unmap(
		g_pVertexBuffer,
		0
		);

// 	PEvent e = EventCenter::getInstance()->createStdEvent(EVENT_SWITCH_TO_SCENE, this, SID_SubScene0, 0);
// 	EventCenter::getInstance()->reportEvent(e);
//	mDirector->switchToScene(SID_SubScene0);

}

void MainScene::render(HV::Renderer *renderer)
{
	ID3D11DeviceContext *renderConext = renderer->getRenderContext();
	ID3DX11EffectMatrixVariable *constantWVP = g_effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	// for one object
	{
		// update constant buffers
		XMMATRIX WVP = g_World * g_View * g_Projection;
		constantWVP->SetMatrix(reinterpret_cast<float*>(&WVP));

		ID3DX11EffectTechnique *tech = mWireframeMode ? g_effect->GetTechniqueByName("WireframeTech") : g_effect->GetTechniqueByName("SolidTech");
		D3DX11_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);
		for (INDEX_T i = 0; i < techDesc.Passes; i++)
		{
			tech->GetPassByIndex(i)->Apply(0, renderConext);
			renderConext->DrawIndexed(g_numIndices, 0, 0); // 36 vertices needed for 12 triangles in a triangle list
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

	ID3D11DeviceContext *renderConext = mDirector->getApp()->getRenderer()->getRenderContext();
	renderConext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderConext->IASetInputLayout(g_pVertexLayout);
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	renderConext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	renderConext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
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
	GeometryGenerator::MeshData meshData;
	GeometryGenerator::createGrid(300, 300, 60, 60, meshData);
	g_numVertices = meshData.Vertices.size();
	std::vector<SimpleVertex> vertices(g_numVertices);
	for (SIZE_T i = 0; i < g_numVertices; i++)
	{
		vertices[i].Pos = (meshData.Vertices)[i].Position;
		updateDynamicVertex(&vertices[i], 0);
	}
	g_numIndices = meshData.Indices.size();
	std::vector<UINT16> indices(g_numIndices);
	for (SIZE_T i = 0; i < g_numIndices; i++)
	{
		indices[i] = (meshData.Indices)[i];
	}

	// Create the vertex buffer
	D3D11_BUFFER_DESC bd;
	OBJ_MEM_CLEAR(bd);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(SimpleVertex) * g_numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

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
	bd.ByteWidth = sizeof(UINT16) * g_numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = &indices[0];

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

void MainScene::updateDynamicVertex(void *v, DECIMAL t)
{
	SimpleVertex *vertex = reinterpret_cast<SimpleVertex*>(v);
	vertex->Pos.y = 0.3f * (vertex->Pos.z * sinf(0.1f * vertex->Pos.x) * sinf(t) + vertex->Pos.x * cosf(0.1f * vertex->Pos.z + t)) * cosf(t);
	if (vertex->Pos.y < -10.0f)
	{
		vertex->Color = XMFLOAT4(0.4f, 0.2f, 0.2f, 1.0f);
	}
	else if (vertex->Pos.y < 5.0f)
	{
		vertex->Color = XMFLOAT4(0.6f, 0.4f, 0.4f, 1.0f);
	}
	else if (vertex->Pos.y < 12.0f)
	{
		vertex->Color = XMFLOAT4(0.8f, 0.6f, 0.6f, 1.0f);
	}
	else if (vertex->Pos.y < 20.0f)
	{
		vertex->Color = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);
	}
	else
	{
		vertex->Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

HV::FACTORY::REGISTER_AUTO_WORKER(MainScene, SID_MainScene, HV::Director::sGetSceneFactory())
// EOF
