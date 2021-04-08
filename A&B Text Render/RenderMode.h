#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "NDI_Send.h"
#include "Window.h"
#include "TCPNet.h"
#include "SimpleText.h"
#include "CustomText.h"
#include "TextSettings.h"
#include "GUI.h"

class RMData
{
public: 
	RMData(size_t serverPort_in, TextSettings settings)
		:
		serverPort(serverPort_in),
		settings(settings)
	{}
	bool CheckRMPortsGood()
	{
		if (serverPort >= std::numeric_limits<unsigned short>::max() || serverPort <= 0U)
			return false;
		else return true;
	}
	unsigned short GetServerPort() const
	{
		return unsigned short(serverPort);
	}
	void SetPortToDefault()
	{
		serverPort = 60000U;
	}
	TextSettings GetSettings() const
	{
		return settings;
	}
private:
	size_t serverPort;
	TextSettings settings;
};

class RenderMode
{
public:
	RenderMode(Window& wnd,RMData& data);
	~RenderMode();
	void Update(Window& wnd);
	void Render(Graphics& gfx);
	bool returnToSetupMode();
private:
	void StringControl(const std::wstring& ctrlStr, Color& colChange);
	

private:
	Time time;
	FPS fps;
	
	bool returnToSetup = false;

	//network
	std::unique_ptr<CustomServer> server;
	int netLooper = 58;
	
	//Gui
	GUI gui;

	//text control
	Color oldFillCol;
	Color oldOutlineColor;
	float currScale;
	bool currSmall = true;
	char buffer[512] = { '\0' };
	std::wstring oldMessage;
	bool holdingLastMsg = false;
	float alpha = 1.0f;
	std::vector<TextSettings> tSet;
	size_t currSettings = 0;

	//simple Text
	Window& wnd;
	CustomText cText;
};


