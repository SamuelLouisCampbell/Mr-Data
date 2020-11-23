#include "App.h"
#include <sstream>
#include "MD_Math.h"
#include "Box.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1920, 1080, L"Mr.Data Window"),
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
	wnd.Gfx().BeginFrame(Colors::Black);


	if (ImGui::Begin("Mode Selection"))
	{
		ImGui::Selectable("CHANGE MODE", &isRenderMode);
		if (!isRenderMode)
		{
			rm.Update(wnd.Gfx());
			ImGui::TextColored({ 255,0,0,255 }, "Render Mode");
		}
		else
		{
			stm.Update(wnd.Gfx());
			ImGui::TextColored({ 0,255,0,255 }, "Input Mode");
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
