#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include "BindableBase.h"
#include "RenderMode.h"
#include "SendTextMode.h"

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
	SendTextMode stm;	
	bool isRenderMode = true;
};

