#include "App.h"
#include <sstream>
#include "MD_Math.h"
#include "Box.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1440, 900, L"A&B Text 1440x900"),
	rm(wnd.Gfx())
{
	
	wnd.Gfx().SetProjection(DirectX::XMMatrixOrthographicOffCenterLH(0.0f, float(wnd.Gfx().GetWindowWidth()),
							float(wnd.Gfx().GetWindowHeight()), 0.0f, 0.0f, 1.0f));

}


int App::Setup()
{
	PROFILE_FUNCTION();
	while (true)
	{
		if (const auto exitCode = Window::ProcessMessages())
		{
			return *exitCode;
		}
		//wnd.SetWndCursor();
			ComposeFrame();
			RenderFrame();
			wnd.Gfx().EndFrame();
	}

}

void App::ComposeFrame()
{
	PROFILE_FUNCTION();
	wnd.Gfx().BeginFrame(clearColor);
	rm.Update(wnd);
}


void App::RenderFrame()
{
	PROFILE_FUNCTION();
	rm.Render(wnd.Gfx());
	rm.SendNDI(wnd.Gfx());
}
