#include "RenderMode.h"
#include <cmath>
#include <functional>

RenderMode::RenderMode(Window& wnd, RMData& data)
	:
	largeScale(data.GetLargeScale()),
	smallScale(data.GetSmallScale()),
	lineSpacing(data.GetSpacing()),
	wnd(wnd),
	cText(wnd.GethWnd(), wnd.Gfx(), L"ABOVEANDBYOND2013")
{	
	server = std::make_unique<CustomServer>(data.GetServerPort());
	server->Start();

	//Text Rendering system
	cText.SetupRenderSystem();
}

RenderMode::~RenderMode()
{
	server->Stop();
	server.release();
	server = nullptr;
}

void RenderMode::Update(Window& wnd)
{
	//run server
	server->Update(-1, false);

	//send healthcheck to clients
	if (netLooper >= 60)
	{
		netLooper = 0;
		server->CheckClientsHealth();
	}
	netLooper++;

	fps.Update(time.Mark());
	std::wstringstream wss;
	wss << "A&B Text. FPS : " << fps.Get();
	wnd.SetTitle(wss.str().c_str());

	if (wnd.Gfx().IsIMGuiEnabled())
	{
		if (ImGui::Begin("Text Controls"))
		{

			//display where message came from in imgui
			
			if (server->GetInfoStream().size() != 0)
			{
				oldInfo = server->GetInfoStream();	
			}
			ImGui::TextColored({ 0.0f, 1.0f, 1.0f, 1.0f }, oldInfo.c_str());
			ImGui::InputFloat("Small text size", &smallScale, 0.02f);
			ImGui::InputFloat("Large text size", &largeScale, 0.02f);
			ImGui::InputFloat("Line spacing", &lineSpacing, 0.02f);
			ImGui::InputFloat("Stroke Width", &strokeWidth, 0.02f);
			ImGui::InputFloat("Offset X", &offsetX, 2.0f);
			ImGui::InputFloat("Offset Y", &offsetY, 2.0f);
			ImGui::InputFloat("Kerning", &kerning, 0.2f);
			ImGui::SliderFloat("Delta Alpha (time)", &deltaAlpha, 0.0f, 3.0f);
			ImGui::SliderFloat("Delta Zoom  (time)", &deltaZoom, 0.01f, 1.0f);
			ImGui::ColorPicker4("Fill Color", &oldFillCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
			ImGui::ColorPicker4("Outine Color", &oldOutlineColor.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
			if (ImGui::Button("Reset Fill Color"))
			{
				fillColor = { Colors::White };
				oldFillCol = { Colors::White };
			}
			if (ImGui::Button("Reset Outline Color"))
			{
				outlineColor = { Colors::White };
				oldOutlineColor = { Colors::White };
			}
			if (ImGui::Button("Large Text."))
			{
				currSmall = false;
			}
			if (ImGui::Button("Small Text."))
			{
				currSmall = true;
			}

		}
		ImGui::End();
	}
}

void RenderMode::Render(Graphics& gfx)
{
	//get messages and parse out control segments
	//std::wstring str = L"NULL....HELLO & You!";//server->GetMessageStream();
	std::wstring str = server->GetMessageStream();

	std::wstring controlString = str.substr(0, 8);
	str.erase(0, 8);

	if (controlString != L"NULL....")
	{
		StringControl(controlString, oldFillCol);
	}
	//update current sizes
	if (currSmall)
	{
		if(currScale >= smallScale)
			currScale -= (smallScale * deltaZoom);
		if(currScale < smallScale)
			currScale = smallScale;
	}
	else
	{
		if(currScale <= largeScale)
			currScale += (largeScale * deltaZoom);
		if (currScale > largeScale)
			currScale = largeScale;
	}

	cText.SetOutlineStroke(strokeWidth);
	cText.SetFontSize(currScale * 72.0f);
	cText.SetLineSpacing(lineSpacing);
	cText.SetKerning(kerning);
	cText.SetOffX(offsetX);
	cText.SetOffY(offsetY);

	if (str.size() > 0)
	{
		alpha = 1.0f;
		fillColor = oldFillCol;
		outlineColor = oldOutlineColor;
		cText.SetTextFillColor(fillColor);
		cText.SetTextOutlineColor(outlineColor);
		cText.Draw(str.c_str());
		oldMessage = str;
		oldFillCol = fillColor;
		oldOutlineColor = outlineColor;
		holdingLastMsg = false;
	}
	else if (str.size() == 0)
	{
		holdingLastMsg = true;

		Color alphaOutline =
		{
			outlineColor.r *= alpha,
			outlineColor.g *= alpha,
			outlineColor.b *= alpha,
			outlineColor.a *= alpha
		};
		Color alphaFill =
		{
			fillColor.r *= alpha,
			fillColor.g *= alpha,
			fillColor.b *= alpha,
			fillColor.a *= alpha
		};
		
		alpha -= 0.001f * deltaAlpha;
		cText.SetTextFillColor(fillColor);
		cText.SetTextOutlineColor(outlineColor);
		cText.Draw(oldMessage.c_str());
	}
	
}

bool RenderMode::returnToSetupMode()
{
	return returnToSetup;
}

void RenderMode::StringControl(const std::wstring& ctrlStr, Color& colChange)
{
	if (ctrlStr == L"RED.....")
		colChange = Colors::Red;
	
	if (ctrlStr == L"GREEN...")
		colChange = Colors::Green;
	
	if (ctrlStr == L"BLUE....")
		colChange = Colors::Blue;

	if (ctrlStr == L"CYAN....")
		colChange = Colors::Cyan;

	if (ctrlStr == L"MAGENTA.")
		colChange = Colors::Magenta;

	if (ctrlStr == L"YELLOW..")
		colChange = Colors::Yellow;

	if (ctrlStr == L"WHITE...")
		colChange = Colors::White;

	if (ctrlStr == L"ORANGE..")
		colChange = Colors::Orange;

	if (ctrlStr == L"LARGE...")
		currSmall = false;
	
	if (ctrlStr == L"SMALL...")
		currSmall = true;
}
