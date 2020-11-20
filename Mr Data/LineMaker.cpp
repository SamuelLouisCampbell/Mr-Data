#include "LineMaker.h"
#include "Line.h"
#include "BindableBase.h"
#include "Gfx_Exception_Macros.h"

LineMaker::LineMaker(Graphics& gfx, DirectX::XMFLOAT3& pos_a, DirectX::XMFLOAT3& pos_b, Color& col)
{
	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};

	auto model = Line::Make<Vertex>(pos_a, pos_b);
	model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));

	AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	//auto pvs = std::make_unique<VertexShader>( gfx,L"VertexShaderFaces.cso" );
	auto pvs = std::make_unique<VertexShader>(gfx, L"ColorIndex_VS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddStaticBind(std::move(pvs));

	//AddStaticBind( std::make_unique<PixelShader>( gfx,L"PixelShaderFaces.cso" ) );
	AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorIndexSingle_PS.cso"));

	AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));
	struct PixelShaderConstants
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[1];
	};
	const PixelShaderConstants cb2 =
	{
		{
			{col.r, col.g, col.b, col.a }
		}
	};

	AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP));

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, 0.5f));
}

void LineMaker::Update(float dt) noexcept
{
}

DirectX::XMMATRIX LineMaker::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat3x3(&mt);
}

void LineMaker::SetTransform(float tX, float tY, float tZ) noexcept
{
	DirectX::XMStoreFloat3x3(&mt, DirectX::XMMatrixScaling(tX, tY, tZ));
}
