#include "App.h"
#include <sstream>

App::App()
	:
	wnd(1280, 720, L"Mr.Data 3D Window")
{}

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
	}

}

void App::ComposeFrame()
{
	
}

void App::RenderFrame()
{
	std::wostringstream oss;
	oss << "Mouse X : " << wnd.mouse.GetPosX() << " Y : " << wnd.mouse.GetPosY();
	wnd.SetTitle(oss.str());

	float cScl = sin(time.Peek()) * 1.0f;
	float cSclCos = cos(time.Peek()) * 1.0f;
	wnd.Gfx().EndFrame();
	wnd.Gfx().ClearBuffer(0.0f, 0.0f,0.0f);

	wnd.Gfx().DrawTestTrtiangle(time.Peek(), cScl, 0.0f, cScl * 3.0f);
	wnd.Gfx().DrawTestTrtiangle(time.Peek(), cScl, cSclCos, 5.0f);
	wnd.Gfx().DrawTestTrtiangle(time.Peek(),
		(float)wnd.mouse.GetPosX() / 640.0f - 1.0f,
		(float)wnd.mouse.GetPosY() / 360.0f - 1.0f,
		cSclCos * 5.0f);
}
