#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "BarebonesWindows.h"
#include "Window.h"
#include <vector>
#include <string>
#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCdmShow)
{
	try 
	{
		LPWSTR name = const_cast<LPWSTR>(TEXT("Mr.Data 3D Engine Window"));
		Window wnd(1280, 720, name);
		int displayDelta = 0;
		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			if (wnd.kbd.KeyIsPressed(VK_MENU))
			{
				MessageBoxA(nullptr, "A button was pressed", "The ALT ", 0);
			}
	

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			while (!wnd.mouse.IsEmpty())
			{
				
				const auto e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::WheelUp)
				{
					displayDelta += 1;
				}
				if (e.GetType() == Mouse::Event::Type::WheelDown)
				{
					displayDelta -= 1;
				}

				std::ostringstream oss;
				oss << "Mouse WheelDelta " << displayDelta;
				wnd.SetTitle(oss.str());
			}
		}
		if (gResult == -1)
		{
			return -1;
		}
		return msg.wParam;
	}
	catch (const MyException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & e)
	{

		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) //catch all
	{
		MessageBox(nullptr, L"No Details Available",L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}