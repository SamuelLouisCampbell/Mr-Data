#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Quad
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		namespace dx = DirectX;

		std::vector<dx::XMFLOAT3> vertices;
		vertices.emplace_back(-1.0f, -1.0f, 1.0f);
		vertices.emplace_back(-1.0f,  1.0f, 1.0f);
		vertices.emplace_back( 1.0f,  1.0f, 1.0f);
		vertices.emplace_back( 1.0f, -1.0f, 1.0f);
	

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}

		return{ std::move(verts),{
			0,1,2,
			2,3,0}
		};
	}
};