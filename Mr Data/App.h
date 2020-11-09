#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include <random>
#include "Time.h"
#include "BindableBase.h"
#include "Box.h"
#include "ImGuiManager.h"

class App
{
public:
	App();
	int Setup();
private:
	void RenderFrame();
	void ComposeFrame();
	
private:
	ImGuiManager imgui_mgr;
	Time time;
	Window wnd;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
	float SpeedDivisor = 3.0f;
};

