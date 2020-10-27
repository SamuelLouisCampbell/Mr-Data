#include "App.h"
#include <sstream>

App::App()
	:
	wnd(1280, 768, L"Mr.Data 3D Window")
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
	
	std::wostringstream oss;
	oss << L"Time Elapsed: " << std::setprecision(3) << time.Peek();
	
	wnd.SetTitle(oss.str());
}

void App::RenderFrame()
{
}
