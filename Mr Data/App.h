#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include "BindableBase.h"
#include "RenderMode.h"
#include "SetupMode.h"


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
	bool rMode = false;
	IMGuiManager img;
	Window wnd;
	RMData rmd;
	std::unique_ptr<RenderMode> rm;
	std::unique_ptr<SetupMode> sm;
	Color clearColor = Colors::Black;

};

