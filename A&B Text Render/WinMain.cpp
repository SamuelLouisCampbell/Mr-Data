#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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
		return App{}.Setup();
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
	_CrtDumpMemoryLeaks();
}