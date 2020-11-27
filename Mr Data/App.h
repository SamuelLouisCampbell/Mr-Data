#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include "BindableBase.h"
#include "RenderMode.h"


class App
{
public:
	App();
	int Setup();
private:
	void RenderFrame();
	void ComposeFrame();
	
private:
	IMGuiManager img;
	Window wnd;
	RenderMode rm;
	Color clearColor = Colors::Black;

};

