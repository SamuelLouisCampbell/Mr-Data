#include "RenderMode.h"

RenderMode::RenderMode(Graphics& gfx)
	:
	txt(gfx, 1.0f, 0.0f, L"assets/arial_64.spritefont"),
	ndi(gfx.GetWindowWidth(), gfx.GetWindowHeight())
{
	centre.x = float(gfx.GetWindowWidth()) / 2.0f;
	centre.y = float(gfx.GetWindowHeight()) / 2.0f;
	txt.SetPos(centre);
}

void RenderMode::Update(Graphics& gfx)
{
	txt.Bind(gfx);

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

void RenderMode::Render(Graphics& gfx)
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
			alpha -= 0.001f * deltaAlpha;
			txt.DrawCentreAlign(oldMessage, lineSpacing);

		}
	}
	catch (const MyException& e)
	{
		MessageBox(nullptr, e.wideWhat(), e.GetType(), MB_OK | MB_ICONASTERISK);
	}
}

void RenderMode::SendNDI(Graphics& gfx)
{
#if NDEBUG
	//Send NDI Frames
	ndi.SendNDIFrame(gfx);
#endif
}
