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
	void EngageMode(bool mode);
	
private:
	bool rMode = true;
	IMGuiManager img;
	Window wnd;
	RMData rmd;
	std::unique_ptr<RenderMode> rm;
	Color clearColor = Colors::Black;

};

