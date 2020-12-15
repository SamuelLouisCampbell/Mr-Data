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
	//setup ndi
	ndi.NDI_Init(wnd.GetWidth(), wnd.GetHeight());
	EngageSetupMode();
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
		if (rm->returnToSetupMode())
		{
			rm.reset();
			rm = nullptr;
			sm = std::make_unique<SetupMode>();
			rMode = false;
		}
	}
	else
	{
		sm->Update();
		if (sm->SetupComplete())
		{
			RMData rmd = sm->GetRMData();
			rm = std::make_unique<RenderMode>(wnd.Gfx(), rmd);
			rMode = true;
			sm.reset();
			sm = nullptr;
		}

	}
}

void App::EngageSetupMode()
{
	sm = std::make_unique<SetupMode>();
}



void App::RenderFrame()
{
	PROFILE_FUNCTION();
	if (rMode)
	{
		rm->Render(wnd.Gfx());
		ndi.SendNDIFrame(wnd.Gfx());
	}

}
	
