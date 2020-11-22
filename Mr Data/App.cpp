#include "App.h"
#include <sstream>
#include "MD_Math.h"
#include "Box.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1280, 720, L"Mr.Data Window"),
	txt(wnd.Gfx(), 1.0f, 0.0f, L"assets/arial_64.spritefont"),
	ndi(1280, 720)
{
	centre.x = float(wnd.GetWidth()) / 2.0f;
	centre.y = float(wnd.GetHeight()) / 2.0f;
	txt.SetPos(centre);
	
	lines.emplace_back(std::make_unique<LineMaker>(wnd.Gfx(), pos_1, pos_2, Red));
	lines.emplace_back(std::make_unique<LineMaker>(wnd.Gfx(), pos_3, pos_4, Cyan));
	lines.emplace_back(std::make_unique<BoxOutline>(wnd.Gfx(), pos_5, pos_6, Red));
	wnd.Gfx().SetProjection(DirectX::XMMatrixOrthographicOffCenterLH(0.0f, float(wnd.Gfx().GetWindowWidth()),
		float(wnd.Gfx().GetWindowHeight()), 0.0f, 0.0f, 1.0f));
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
	
	Color clearCol = { 0.0f, 0.0f, 0.0f, 1.0f};
	wnd.Gfx().BeginFrame(clearCol);
	
	if (ImGui::Begin("Text Controls"))
	{
		std::stringstream ss;
		ss << "Time :" << time.Mark();
		ImGui::Text(ss.str().c_str());
		ImGui::SliderFloat("Scale", &scale, 0.0f, 3.0f);
		ImGui::SliderFloat("Rotation", &rotation, -3.14159f, +3.14159f);
		ImGui::SliderFloat("Line Spacing", &lineSpacing, 0.0f, 3.0f);
		ImGui::SliderFloat("Delta Alpha (time)", &deltaAlpha, 0.0f, 3.0f);
		ImGui::ColorPicker4("Color", &oldTextCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);
		if (ImGui::Button("Reset"))
		{
			textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
			scale = 1.0f;
			rotation = 0.0f;
			lineSpacing = 1.5f;
		}
		txt.setScale(scale);
		txt.setRotation(rotation);
		if (!holdingLastMsg)
		{
			txt.setColor(oldTextCol);
		}
		ImGui::InputTextMultiline("Input Text: ", buffer, sizeof(buffer));

	}
	ImGui::End();
}


void App::RenderFrame()
{
	size_t size = sizeof(buffer) + 1;
	static wchar_t wbuffer[512];
	size_t outSize;
	mbstowcs_s(&outSize, wbuffer, size, buffer, size);
	std::wstring message = wbuffer;


	try
	{
		if (message.size() > 0)
		{
			alpha = 1.0f;
			textCol = oldTextCol;
			txt.setColor(textCol);
			txt.DrawCentreAlign(message, lineSpacing);
			oldMessage = message;
			oldTextCol = textCol;
			holdingLastMsg = false;
		}
		else if (message.size() == 0)
		{
			holdingLastMsg = true;

			Color preMulAplpha =
			{
				textCol.r *= alpha,
				textCol.g *= alpha,
				textCol.b *= alpha,
				textCol.a *= alpha
			};
			txt.setColor(preMulAplpha);
			alpha -= 0.001 * deltaAlpha;
			txt.DrawCentreAlign(oldMessage, lineSpacing);

		}
	}
	catch (const MyException& e)
	{
		MessageBox(nullptr, e.wideWhat(), e.GetType(), MB_OK | MB_ICONASTERISK);
	}
	
#if NDEBUG
	//Send NDI Frames
	ndi.SendNDIFrame(wnd.Gfx());
#endif

	for (auto& line : lines)
	{
		line->Draw(wnd.Gfx());
	}

}
