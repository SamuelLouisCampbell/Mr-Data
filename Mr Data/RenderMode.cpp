#include "RenderMode.h"
#include <cmath>

RenderMode::RenderMode(Graphics& gfx)
	:
	txt(gfx, 1.0f, 0.0f, L"assets/arial_128.spritefont"),
	ndi(gfx.GetWindowWidth(), gfx.GetWindowHeight()),
	udp_s(6000),
	echoClient(5000)
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
		//display where message came from in imgui
		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, udp_s.GetStatusReadout().c_str());
		ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, udp_s.GetMessageForGUI().c_str());
		ImGui::InputFloat("Small text size", &smallScale, 0.02f);
		ImGui::InputFloat("Small line spacing", &lineSpacingSmall, 0.02f);
		ImGui::InputFloat("Large text size", &largeScale, 0.02f);
		ImGui::InputFloat("Large line spacing", &lineSpacingLarge, 0.02f);
		ImGui::SliderFloat("Delta Alpha (time)", &deltaAlpha, 0.0f, 3.0f);
		ImGui::SliderFloat("Delta Zoom  (time)", &deltaZoom, 0.01f, 1.0f);
		ImGui::ColorPicker3("Color", &oldTextCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel);
		if (ImGui::Button("Reset"))
		{
			textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		txt.setScale(currScale);
		txt.setRotation(rotation);
		
	}
	ImGui::End();
}

void RenderMode::Render(Graphics& gfx)
{
	//get messages and parse out control segments
	std::string str = udp_s.GetNetworkMessage();
	std::string controlString = str.substr(0, 8);
	str.erase(0, 8);

	if (controlString != "NULL....")
	{
		StringControl(controlString, oldTextCol);
	}

	//update current sizes
	if (currSmall)
	{
		if(currScale >= smallScale)
		{
			currScale -= (smallScale * deltaZoom);
		}
		if(currLineSpacing >= lineSpacingSmall)
		{
			currLineSpacing -= (lineSpacingSmall *deltaZoom);
		}
		if(currScale < smallScale)
			currScale = smallScale;
		if (currLineSpacing < lineSpacingSmall)
			currLineSpacing = lineSpacingSmall;
	}
	else
	{
		if(currScale <= largeScale)
		{
			currScale += (largeScale * deltaZoom);
		}
		if(currLineSpacing <= lineSpacingLarge)
		{
			currLineSpacing += (lineSpacingLarge * deltaZoom);
		}
		if (currScale > largeScale)
			currScale = largeScale;
		if (currLineSpacing > lineSpacingLarge)
			currLineSpacing = lineSpacingLarge;
	}
	txt.setScale(currScale);

	size_t size = str.size() + 1;
 	static wchar_t wbuffer[512];
	size_t outSize;
	mbstowcs_s(&outSize, wbuffer, size, str.c_str(), size); // convert to wsting
	std::wstring message = wbuffer;
	echoClient.UDP_Send(str);

	try
	{
		
		if (message.size() > 0)
		{
			alpha = 1.0f;
			textCol = oldTextCol;
			txt.setColor(textCol);
			txt.DrawCentreAlign(message, currLineSpacing);
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
			txt.DrawCentreAlign(oldMessage, currLineSpacing);

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

void RenderMode::StringControl(const std::string& ctrlStr, Color& colChange)
{
	if (ctrlStr == "RED.....")
		colChange = Colors::Red;
	
	if (ctrlStr == "GREEN...")
		colChange = Colors::Green;
	
	if (ctrlStr == "BLUE....")
		colChange = Colors::Blue;

	if (ctrlStr == "CYAN....")
		colChange = Colors::Cyan;

	if (ctrlStr == "MAGENTA.")
		colChange = Colors::Magenta;

	if (ctrlStr == "YELLOW..")
		colChange = Colors::Yellow;

	if (ctrlStr == "WHITE...")
		colChange = Colors::White;

	if (ctrlStr == "ORANGE..")
		colChange = Colors::Orange;

	if (ctrlStr == "LARGE...")
	{
		currSmall = false;
	}

	if (ctrlStr == "SMALL...")
	{
		currSmall = true;
	}
}
