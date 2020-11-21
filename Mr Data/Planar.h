#pragma once
#include "DrawableBase.h"


class Planar : public DrawableBase<Planar>
{
public:
	Planar(Graphics& gfx, const DirectX::XMFLOAT3& pos_a, const DirectX::XMFLOAT3& pos_b);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SetTransform(float tX, float tY, float tZ) noexcept override;
private:
	DirectX::XMFLOAT3X3 mt;
};