#pragma once
#include "DrawableBase.h"


class Planar : public DrawableBase<Planar>
{
public:
	Planar(Graphics& gfx);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SetTransform(float tX, float tY, float tZ) noexcept override;
private:
	DirectX::XMFLOAT3X3 mt;
};