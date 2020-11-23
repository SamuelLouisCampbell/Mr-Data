#include "SendTextMode.h"
#include "LineMaker.h"
#include "BoxOutline.h"
#include "BoxFill.h"


SendTextMode::SendTextMode(Graphics& gfx)
	:
	smallText(gfx, 1.0f, 0.0f, L"assets/arial_32.spritefont"),
	largeText(gfx, 1.0f, 0.0f, L"assets/arial_64.spritefont"),
	sysText(gfx, 1.0f, 0.0f, L"assets/consolas_16.spritefont")

{
	sysText.Bind(gfx);

	pos_1 = { 200.0f,  100.0f, 1.0f };
	pos_2 = { gfx.GetWindowWidth() - 200.0f, (gfx.GetWindowHeight() / 2.0f) - 50.0f, 1.0f };
	pos_3 = { 200.0f, (gfx.GetWindowHeight() / 2.0f) + 50.0f, 1.0f };
	pos_4 = { gfx.GetWindowWidth() - 200.0f, gfx.GetWindowHeight() - 100.0f, 1.0f };

	borders.emplace_back(std::make_unique<BoxOutline>(gfx, pos_1, pos_2, Colors::Grey));
	borders.emplace_back(std::make_unique<BoxFill>(gfx, pos_1, pos_2, Colors::Black));
	borders.emplace_back(std::make_unique<BoxOutline>(gfx, pos_3, pos_4, Colors::Grey));
	borders.emplace_back(std::make_unique<BoxFill>(gfx, pos_3, pos_4, Colors::Black));

	
}

void SendTextMode::Update(Graphics& gfx)
{
}

void SendTextMode::Render(Graphics& gfx)
{
	//User Interface text
	{ 
		sysText.SetPos({ pos_1.x, pos_1.y });
		sysText.Draw(L"Small Text");
		sysText.SetPos({ pos_3.x, pos_3.y });
		sysText.Draw(L"Large Text");
	} 


	for (auto& line : borders)
	{
		line->Draw(gfx);
	}
}