#pragma once
#include "Window.h"
#include <vector>
#include <string>

class App
{
public:
	App(int width, int height, std::wstring name);
	void ComposeFrame();
	void RenderFrame();
private:
	void DoFrame();

private:
	Window wnd;
	int width;
	int height;
	std::string name;

};

