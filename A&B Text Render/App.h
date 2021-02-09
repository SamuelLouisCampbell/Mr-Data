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
	void EngageSetupMode();
private:
	//NDI stuff
	NDI_Send ndi;

	bool rMode = false;
	IMGuiManager img;
	Window wnd;
	std::unique_ptr<RenderMode> rm = nullptr;
	std::unique_ptr<SetupMode> sm = nullptr;
	Color clearColor = Colors::Alpha;

};

