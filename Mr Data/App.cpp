#include "App.h"

App::App(int width, int height, std::wstring name)
	:
	wnd(width, height, name)
{}

int App::Setup()
{
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	if (gResult == -1)
	{
		return -1;
	}
	return msg.wParam;
}

void App::ComposeFrame()
{
}

void App::RenderFrame()
{
}
