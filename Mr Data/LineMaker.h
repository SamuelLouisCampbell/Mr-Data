#pragma once
#include "DrawableBase.h"

class LineMaker : public DrawableBase<LineMaker>
{
public:
	LineMaker(Graphics& gfx, const DirectX::XMFLOAT3& pos_a, const DirectX::XMFLOAT3& pos_b, const Color& col);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SetTransform(float tX, float tY, float tZ) noexcept override;
private:
	DirectX::XMFLOAT3X3 mt;
};


