#pragma once
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();
	void NewFrame();
	void RenderFrame();
};

