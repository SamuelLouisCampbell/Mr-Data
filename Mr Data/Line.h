#pragma once
#pragma once
#include "IndexedLineList.h"
#include <DirectXMath.h>

class Line
{
public:
	template<class V>
	static IndexedLineList<V> Make(DirectX::XMFLOAT3 pos_a, DirectX::XMFLOAT3 pos_b)
	{
		namespace dx = DirectX;

		std::vector<dx::XMFLOAT3> vertices;
		vertices.emplace_back(pos_a);
		vertices.emplace_back(pos_b);

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}

		return{ std::move(verts),{0,1}
		};
	}
};
