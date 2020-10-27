#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "App.h"
#include <string>


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCdmShow)
{
	try 
	{
		const wchar_t* name = L"Mr.Data 3D Engine Window";
		Window wnd(1280, 720, name);
	
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
	catch (const MyException& e)
	{
		MessageBox(nullptr, e.wideWhat(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
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