#include "SendTextMode.h"
#include "LineMaker.h"
#include "BoxOutline.h"
#include "BoxFill.h"


SendTextMode::SendTextMode(Graphics& gfx)
	:
	smallText(gfx, 1.0f, 0.0f, L"assets/arial_32.spritefont"),
	largeText(gfx, 1.0f, 0.0f, L"assets/arial_64.spritefont"),
	systemText(gfx, 1.0f, 0.0f, L"assets/consolas_16.spritefont")

{
	DirectX::XMFLOAT3 pos_1 = { 100.0f, 100.0f, 1.0f };
	DirectX::XMFLOAT3 pos_2 = { 300.0f, 300.0f, 1.0f };

	borders.emplace_back(std::make_unique<BoxOutline>(gfx, pos_1, pos_2, Colors::Teal));
}

void SendTextMode::Update(Graphics& gfx)
{
}

void SendTextMode::Render(Graphics& gfx)
{
	for (auto& line : borders)
	{
		line->Draw(gfx);
	}
}
