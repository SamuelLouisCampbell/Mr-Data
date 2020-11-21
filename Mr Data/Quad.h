#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Quad
{
public:
	template<class V>
	static IndexedTriangleList<V> Make(const DirectX::XMFLOAT3& pos_a, const DirectX::XMFLOAT3& pos_b)
	{
		namespace dx = DirectX;

		std::vector<dx::XMFLOAT3> vertices;
		vertices.emplace_back(pos_a);
		vertices.emplace_back(pos_b.x, pos_a.y, pos_a.z);
		vertices.emplace_back(pos_b);
		vertices.emplace_back(pos_a.x, pos_b.y, pos_b.z);
	

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