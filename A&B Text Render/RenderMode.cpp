#include "RenderMode.h"
#include <cmath>
#include <functional>

RenderMode::RenderMode(Window& wnd, RMData& data)
	:
	largeScale(data.GetLargeScale()),
	smallScale(data.GetSmallScale()),
	lineSpacingLarge(data.GetLargeSpacing()),
	lineSpacingSmall(data.GetSmallSpacing()),
	wnd(wnd),
	st(wnd.GethWnd(),wnd.Gfx(), {1.0f,1.0f,1.0f,1.0f}, L"ABOVEANDBYOND2013")
{	
	server = std::make_unique<CustomServer>(data.GetServerPort());
	server->Start();

	//Text Rendering system
	st.SetupRenderSystem();

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

	st.SetFontSize(currScale*72.0f);

	size_t size = str.size() + 1;
 	static wchar_t wbuffer[512];
	size_t outSize;
	mbstowcs_s(&outSize, wbuffer, size, str.c_str(), size); // convert to wsting
	std::wstring message = wbuffer;
	
	
	if (message.size() > 0)
	{
		alpha = 1.0f;
		textCol = oldTextCol;
		
		st.Draw(message.c_str());
		st.SetTextColor(textCol);
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
		
		alpha -= 0.001f * deltaAlpha;
		st.Draw(oldMessage.c_str());
		st.SetTextColor(preMulAplpha);

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
