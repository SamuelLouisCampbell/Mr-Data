#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include <random>
#include "Time.h"
#include "BindableBase.h"
#include "ImGuiManager.h"
#include "TextNode.h"

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
	TextNode txt;
	DirectX::SimpleMath::Vector2 centre = { 0.0f, 0.0f };
	Color textCol;
};

