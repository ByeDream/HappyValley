/************************************************************************/
/* The Vertex Shader Implementation Base On D3D11
/*
/* ByeDream
/* Dec-28-2015
/************************************************************************/
#ifndef __VERTEX_SHADER_D3D11_H__
#define __VERTEX_SHADER_D3D11_H__

#include "Framework/Modules/Rendering/VertexShader.h"

namespace HV
{
	class VertexShaderD3D11 : public VertexShader
	{
	public:
		VertexShaderD3D11() {}
		virtual ~VertexShaderD3D11() {}

		virtual void loadFromFile(const char *filePath) {}
		virtual void loadFromMemory() {}

	private:
	};
}


#endif // !__VERTEX_SHADER_D3D11_H__
