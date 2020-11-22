#include "App.h"
#include <sstream>
#include "MD_Math.h"
#include "Box.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1280, 720, L"Mr.Data Window"),
	rm(wnd.Gfx())
	
{
	lines.emplace_back(std::make_unique<LineMaker>(wnd.Gfx(), pos_1, pos_2, Colors::Red));
	lines.emplace_back(std::make_unique<LineMaker>(wnd.Gfx(), pos_3, pos_4, Colors::Orange));
	lines.emplace_back(std::make_unique<BoxOutline>(wnd.Gfx(), pos_5, pos_6,Colors::Teal));
	lines.emplace_back(std::make_unique<BoxFill>(wnd.Gfx(), pos_5, pos_4, Colors::Cyan));
	wnd.Gfx().SetProjection(DirectX::XMMatrixOrthographicOffCenterLH(0.0f, float(wnd.Gfx().GetWindowWidth()),
		float(wnd.Gfx().GetWindowHeight()), 0.0f, 0.0f, 1.0f));
}

int App::Setup()
{
	while (true)
	{
		if (const auto exitCode = Window::ProcessMessages())
		{
			return *exitCode;
		}
	
		ComposeFrame();
		RenderFrame();
		wnd.Gfx().EndFrame();
	}

}

void App::ComposeFrame()
{	
	wnd.Gfx().BeginFrame(Colors::Black);
	rm.Update(wnd.Gfx());
}


void App::RenderFrame()
{
	rm.Render(wnd.Gfx());
	for (auto& line : lines)
	{
		line->Draw(wnd.Gfx());
	}
	rm.SendNDI(wnd.Gfx());

}
