#include "RenderMode.h"
#include <cmath>
#include <functional>

RenderMode::RenderMode(Window& wnd, RMData& data)
	:
	largeScale(data.GetLargeScale()),
	smallScale(data.GetSmallScale()),
	lineSpacing(data.GetSpacing()),
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
			ImGui::InputFloat("Large text size", &largeScale, 0.02f);
			ImGui::InputFloat("Line spacing", &lineSpacing, 0.02f);
			ImGui::SliderFloat("Delta Alpha (time)", &deltaAlpha, 0.0f, 3.0f);
			ImGui::SliderFloat("Delta Zoom  (time)", &deltaZoom, 0.01f, 1.0f);
			ImGui::ColorPicker3("Color", &oldTextCol.r, ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
			if (ImGui::Button("Reset Color"))
			{
				textCol = { Colors::White };
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
	std::wstring str = L"NULL....Hello You!";//server->GetMessageStream();
	
	
	std::wstring controlString = str.substr(0, 8);
	str.erase(0, 8);

	if (controlString != L"NULL....")
	{
		StringControl(controlString, oldTextCol);
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

	st.SetFontSize(currScale*72.0f);
	st.SetLineSpacing(lineSpacing);
	
	
	if (str.size() > 0)
	{
		alpha = 1.0f;
		textCol = oldTextCol;
		
		st.Draw(str.c_str());
		st.SetTextColor(textCol);
		oldMessage = str;
		oldTextCol = textCol;
		holdingLastMsg = false;
	}
	else if (str.size() == 0)
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
