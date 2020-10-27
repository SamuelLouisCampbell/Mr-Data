#pragma once
#include "Window.h"
#include <vector>
#include <string>

class App
{
public:
	App(int width, int height, std::wstring name);
	int Setup();
	void RenderFrame();
private:
	void ComposeFrame();


private:
	Window wnd;
	int width;
	int height;
	std::string name;

};

