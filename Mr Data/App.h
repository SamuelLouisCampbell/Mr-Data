#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include "Time.h"

class App
{
public:
	App();
	int Setup();
private:
	void RenderFrame();
	void ComposeFrame();
	
private:
	Window wnd;
	Time time;

};

