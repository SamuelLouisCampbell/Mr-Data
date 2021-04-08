#include "RenderMode.h"
#include <cmath>
#include <functional>

RenderMode::RenderMode(Window& wnd, RMData& data)
	:
	wnd(wnd),
	cText(wnd.GethWnd(), wnd.Gfx(), L"ABOVEANDBYOND2013"), 
	gui(tSet, data.GetSettings())
{	
	defaultTextSettings = data.GetSettings();
	for (int i = 0; i < maxSettings; i++)
	{
		tSet.push_back(defaultTextSettings);
	}
	server = std::make_unique<CustomServer>(data.GetServerPort());
	server->Start();

	oldFillCol = tSet[currSettings].currFillCol;
	oldOutlineColor = tSet[currSettings].currOutlineCol;
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
		size_t oldSettingIndex = currSettings;
		gui.ControlWindow(server->GetInfoStream(), currSmall, currSettings);
		if (currSettings > (tSet.size() -1))
		{
			tSet.push_back(defaultTextSettings);
			oldFillCol = defaultTextSettings.currFillCol;
			oldOutlineColor = defaultTextSettings.currOutlineCol;
		}
		gui.PrestEditorWindow(currSettings, oldFillCol, oldOutlineColor);
		if (currSettings != oldSettingIndex)
		{
			oldFillCol = tSet[currSettings].currFillCol;
			oldOutlineColor = tSet[currSettings].currOutlineCol;
		}
		
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
		if(currScale >= tSet[currSettings].smallScale)
			currScale -= (tSet[currSettings].smallScale * tSet[currSettings].deltaZoom);
		if(currScale < tSet[currSettings].smallScale)
			currScale = tSet[currSettings].smallScale;
	}
	else
	{
		if(currScale <= tSet[currSettings].largeScale)
			currScale += (tSet[currSettings].largeScale * tSet[currSettings].deltaZoom);
		if (currScale > tSet[currSettings].largeScale)
			currScale = tSet[currSettings].largeScale;
	}

	cText.SetOutlineStroke(tSet[currSettings].strokeWidth);
	cText.SetFontSize(currScale * 72.0f);
	cText.SetLineSpacing(tSet[currSettings].lineSpacing);
	cText.SetKerning(tSet[currSettings].kerning);
	cText.SetOffX(tSet[currSettings].offsetX);
	cText.SetOffY(tSet[currSettings].offsetY);

	if (str.size() > 0)
	{
		alpha = 1.0f;
		tSet[currSettings].currFillCol = oldFillCol;
		tSet[currSettings].currOutlineCol = oldOutlineColor;
		cText.SetTextFillColor(tSet[currSettings].currFillCol);
		cText.SetTextOutlineColor(tSet[currSettings].currOutlineCol);
		cText.Draw(str.c_str());
		oldMessage = str;
		oldFillCol = tSet[currSettings].currFillCol;
		oldOutlineColor = tSet[currSettings].currOutlineCol;
		tSet[currSettings].oldFillCol = oldFillCol;
		tSet[currSettings].oldOutlineCol = oldOutlineColor;
		holdingLastMsg = false;
	}
	else if (str.size() == 0)
	{
		holdingLastMsg = true;

		Color alphaOutline =
		{
			tSet[currSettings].currOutlineCol.r *= alpha,
			tSet[currSettings].currOutlineCol.g *= alpha,
			tSet[currSettings].currOutlineCol.b *= alpha,
			tSet[currSettings].currOutlineCol.a *= alpha
		};
		Color alphaFill =
		{
			tSet[currSettings].currFillCol.r *= alpha,
			tSet[currSettings].currFillCol.g *= alpha,
			tSet[currSettings].currFillCol.b *= alpha,
			tSet[currSettings].currFillCol.a *= alpha
		};
		
		alpha -= 0.001f * tSet[currSettings].deltaAlpha;
		cText.SetTextFillColor(tSet[currSettings].currFillCol);
		cText.SetTextOutlineColor(tSet[currSettings].currOutlineCol);
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
