#include "App.h"
#include <sstream>
#include "MD_Math.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1440, 900, L"A&B Text 1440x900")
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixOrthographicOffCenterLH(0.0f, float(wnd.Gfx().GetWindowWidth()),
							float(wnd.Gfx().GetWindowHeight()), 0.0f, 0.0f, 1.0f));
	EngageMode(rMode);
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
	if (rMode)
	{
		rm->Update(wnd);
	}
	else
	{
		sm->Update();
	}
}

void App::EngageMode(bool mode)
{

	if (mode == false)
	{
		sm = std::make_unique<SetupMode>();
	}
	else
	{
		rmd.clientIP = "127.0.0.1";
		rmd.clientPort = 5000U;
		rmd.serverPort = 6000U;
		rm = std::make_unique<RenderMode>(wnd.Gfx(), rmd);
	}
}



void App::RenderFrame()
{
	PROFILE_FUNCTION();
	if (rMode)
	{
		rm->Render(wnd.Gfx());
		rm->SendNDI(wnd.Gfx());
	}
	else
	{
		sm->Render();
	}
}
	
