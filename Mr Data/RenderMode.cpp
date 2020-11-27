#include "RenderMode.h"

RenderMode::RenderMode(Graphics& gfx)
	:
	txt(gfx, 1.0f, 0.0f, L"assets/arial_64.spritefont"),
	ndi(gfx.GetWindowWidth(), gfx.GetWindowHeight()),
	udp_s(6000),
	udp_c(5000)
{
	centre.x = float(gfx.GetWindowWidth()) / 2.0f;
	centre.y = float(gfx.GetWindowHeight()) / 2.0f;
	txt.SetPos(centre);
}

void RenderMode::Update(Graphics& gfx)
{
	txt.Bind(gfx);

	//get messages from network.
	udp_s.Recieve();

	if (ImGui::Begin("Text Controls"))
	{
		std::stringstream ss;
		ss << "Time :" << time.Mark();
		ImGui::Text(ss.str().c_str());
		ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, udp_s.GetStatusReadout().c_str());
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
		//ImGui::Text(udp_s.GetStatusReadout().c_str());

	}
	ImGui::End();
}

void RenderMode::Render(Graphics& gfx)
{
	std::string str = udp_s.GetNetworkMessage();
	size_t size = str.size() + 1;
 	static wchar_t wbuffer[512];
	size_t outSize;
	mbstowcs_s(&outSize, wbuffer, size, str.c_str(), size);
	std::wstring message = wbuffer;
	udp_c.UDP_Send(str);

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
