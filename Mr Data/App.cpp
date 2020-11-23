#include "App.h"
#include <sstream>
#include "MD_Math.h"
#include "Box.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1440, 900, L"A&B Text 1440x900"),
	rm(wnd.Gfx()),
	stm(wnd.Gfx())
{
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
	wnd.Gfx().BeginFrame(clearColor);

	if (ImGui::Begin("Mode Selection"))
	{
		ImGui::Selectable("CHANGE MODE", &isRenderMode);
		if (!isRenderMode)
		{
			rm.Update(wnd.Gfx());
			ImGui::TextColored({ 255,0,0,255 }, "Render Mode");
			clearColor = Colors::Black;
		}
		else
		{
			stm.Update(wnd.Gfx());
			ImGui::TextColored({ 0,255,0,255 }, "Input Mode");
			clearColor = Colors::Navy;
		}
	}
	
	ImGui::End();

	
	
}


void App::RenderFrame()
{
	if (!isRenderMode)
	{
		rm.Render(wnd.Gfx());
		rm.SendNDI(wnd.Gfx());
	}
	else
	{
		stm.Render(wnd.Gfx());
	}

	
}
