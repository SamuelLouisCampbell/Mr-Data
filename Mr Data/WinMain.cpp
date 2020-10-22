#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCdmShow)
{

	const auto* pClassName = L"Mr. Data";
	//window class options struct
	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = 0x0020 | 0x0200;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	
	//register windows class
	RegisterClassExW(&wc);

	//creat class instances
	HWND hWnd = CreateWindowExW(
		0, pClassName,L"Mr.Data Window", 
		WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		200,200,1280,720,
		nullptr,nullptr,hInstance,nullptr
	);

	//show the Window
	ShowWindow(hWnd, SW_SHOW);

	//Get messages


	while (true);
	return 0;
}