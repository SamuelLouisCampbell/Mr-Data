#include "RenderMode.h"
#include <cmath>
#include <functional>

RenderMode::RenderMode(Graphics& gfx, RMData& data)
	:
	txt(gfx, 1.0f, 0.0f, L"assets/arial_128.spritefont"),
	largeScale(data.GetLargeScale()),
	smallScale(data.GetSmallScale()),
	lineSpacingLarge(data.GetLargeSpacing()),
	lineSpacingSmall(data.GetSmallSpacing())
{
	centre.x = float(gfx.GetWindowWidth()) / 2.0f;
	centre.y = float(gfx.GetWindowHeight()) / 2.0f;
	txt.SetPos(centre);
	txt.Bind(gfx);
	server = std::make_unique<CustomServer>(data.GetServerPort());
	server->Start();
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
			ImGui::InputFloat("Small line spacing", &lineSpacingSmall, 0.02f);
			ImGui::InputFloat("Large text size", &largeScale, 0.02f);
			ImGui::InputFloat("Large line spacing", &lineSpacingLarge, 0.02f);
			ImGui::SliderFloat("Delta Alpha (time)", &deltaAlpha, 0.0f, 3.0f);
			ImGui::SliderFloat("Delta Zoom  (time)", &deltaZoom, 0.01f, 1.0f);
			ImGui::ColorPicker3("Color", &oldTextCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
			if (ImGui::Button("Reset Color"))
			{
				textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
				oldTextCol = { Colors::White };
			}
			if (ImGui::Button("Large Text."))
			{
				currSmall = false;
			}
			if (ImGui::Button("Small Text."))
			{
				currSmall = true;
			}
			txt.setScale(currScale);
			txt.setRotation(rotation);
		}
		ImGui::End();
	}
}

void RenderMode::Render(Graphics& gfx)
{
	//get messages and parse out control segments
	std::string str = server->GetMessageStream();
	
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

	//size_t size = str.size() + 1;
 //	static wchar_t wbuffer[512];
	//size_t outSize;
	//mbstowcs_s(&outSize, wbuffer, size, str.c_str(), size); // convert to wsting
	//std::wstring message = wbuffer;

	try
	{
		if (str.size() > 1)
		{
			alpha = 1.0f;
			textCol = oldTextCol;
			txt.setColor(textCol);
			txt.DrawCentreAlign(str, currLineSpacing);
			oldMessage = str;
			oldTextCol = textCol;
			holdingLastMsg = false;
		}
		else if (str.size() == 1)
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

bool RenderMode::returnToSetupMode()
{
	return returnToSetup;
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
