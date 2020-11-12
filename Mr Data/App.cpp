#include "App.h"
#include <sstream>
#include "Melon.h"
#include "Pyramid.h"
#include "MD_Math.h"
#include "Box.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1280, 720, L"Mr.Data 3D Window"),
	txt(wnd.Gfx(),1.0f, 0.0f, L"assets/arial_64.spritefont")
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, float(wnd.GetHeight()) / float(wnd.GetWidth()), 0.2f, 50.0f));
	centre.x = float(wnd.GetWidth()) / 2.0f;
	centre.y = float(wnd.GetHeight()) / 2.0f;
	txt.SetPos(centre);
}

int App::Setup()
{
	while (true)
	{
		if (const auto exitCode = Window::ProcessMessages())
		{
			return *exitCode;
		}

		ComposeFrame();
		RenderFrame();
		wnd.Gfx().EndFrame();
	}

}

void App::ComposeFrame()
{	
	txt.Bind(wnd.Gfx());
		
	Color clearCol = { 0.0f, 0.0f, 0.0f };
	wnd.Gfx().BeginFrame(clearCol);

	

	if (ImGui::Begin("Text Controller"))
	{
		ImGui::SliderFloat("Scale", &scale, 0.0f, 3.0f);
		ImGui::SliderFloat("Rotation", &rotation, -3.14159f, +3.14159f);
		ImGui::SliderFloat("Line Spacing", &lineSpacing, 0.0f, 3.0f);
		ImGui::ColorPicker4("Color", &textCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);
		if (ImGui::Button("Reset"))
		{
			textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
			scale = 1.0f;
			rotation = 0.0f;
			lineSpacing = 1.5f;
		}
		txt.setScale(scale);
		txt.setRotation(rotation);
		txt.setColor(textCol);

		ImGui::InputTextMultiline("Input Text: ", buffer, sizeof(buffer));


	}
	ImGui::End();

}

void App::RenderFrame()
{
	//wnd.Gfx().Draw2DTextures();
	size_t size = sizeof(buffer) + 1;
	static wchar_t wbuffer[512];
	size_t outSize;
	mbstowcs_s(&outSize, wbuffer, size, buffer, size);
	std::wstring message = wbuffer;
	
	if(message.size() > 0)
	{
		txt.DrawCentreAlign(message, lineSpacing);
		oldMessage = message;
	}
	else if(message.size() == 0)
	{
	
		txt.DrawCentreAlign(oldMessage, lineSpacing);
		
	}

}
