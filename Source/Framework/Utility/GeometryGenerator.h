/************************************************************************/
/* The Geometry Maker For Procedurally Generating The Common Geometry
/*
/* ByeDream
/* Jan-04-2015
/************************************************************************/
#ifndef __GEOMETRY_GENERATOR_H__
#define __GEOMETRY_GENERATOR_H__

#include "Framework/Math/Math.h"

namespace HV
{
	class GeometryGenerator
	{
	public:
		// defines the vertex class for internal usage 
		struct Vertex
		{
			Vertex(){}
			Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
				: Position(p), Normal(n), TangentU(t), TexC(uv){}
			Vertex(
				float px, float py, float pz,
				float nx, float ny, float nz,
				float tx, float ty, float tz,
				float u, float v)
				: Position(px, py, pz), Normal(nx, ny, nz),
				TangentU(tx, ty, tz), TexC(u, v){}

			XMFLOAT3 Position;
			XMFLOAT3 Normal;
			XMFLOAT3 TangentU;
			XMFLOAT2 TexC;
		};

		// defines the mesh data class for internal usage
		typedef std::vector<Vertex> VertexList;
		typedef std::vector<UINT16> IndexList;
		struct MeshData
		{
			VertexList Vertices;
			IndexList Indices;
		};

		static void createBox(DECIMAL width, DECIMAL height, DECIMAL depth, MeshData& meshData);
		static void createSphere(DECIMAL radius, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
		static void createGeosphere(DECIMAL radius, UINT32 numSubdivisions, MeshData& meshData);
		static void createCylinder(DECIMAL bottomRadius, DECIMAL topRadius, DECIMAL height, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
		static void createGrid(DECIMAL width, DECIMAL depth, UINT32 m, UINT32 n, MeshData& meshData);
		static void createFullscreenQuad(MeshData& meshData); // full screen quad in NDC space

	private:
		static void subdivide(MeshData& meshData);
		static void buildCylinderTopCap(DECIMAL bottomRadius, DECIMAL topRadius, DECIMAL height, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
		static void buildCylinderBottomCap(DECIMAL bottomRadius, DECIMAL topRadius, DECIMAL height, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
	};
}

#endif // !__GEOMETRY_GENERATOR_H__