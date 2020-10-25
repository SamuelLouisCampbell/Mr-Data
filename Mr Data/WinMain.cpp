#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "BarebonesWindows.h"
#include "Window.h"
#include <vector>
#include <string>

//void ConvertToWide(const char* mbstr, wchar_t* buffer_in)
//{
//	std::mbstate_t state = std::mbstate_t();
//	std::size_t len = 1 + std::mbsrtowcs(NULL, &mbstr, 0, &state);
//	std::vector<wchar_t> wstr(len);
//	std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state);
//	for (int i = 0; i <  wstr.size(); i++)
//	{
//		buffer_in[i] = wstr[i];
//	}
//}

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