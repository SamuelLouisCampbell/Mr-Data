#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class IMGuiManager
{
public:
	IMGuiManager()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
	}
	~IMGuiManager()
	{
		ImGui::DestroyContext();
	}
	//void NewFrame()
	//{
	//	ImGui_ImplDX11_NewFrame();
	//	ImGui_ImplWin32_NewFrame();
	//	ImGui::NewFrame();
	//}
	//void RenderFrame()
	//{
	//	ImGui::Render();
	//	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	//}
};

