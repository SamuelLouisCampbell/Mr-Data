#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "NDI_Send.h"
#include "Window.h"
#include "TCPNet.h"
#include "SimpleText.h"


class RMData
{
public: 
	RMData(size_t serverPort_in, float largeScale, 
		   float smallScale, float lineSpacingLarge, float lineSpacingSmall)
		:
		serverPort(serverPort_in),
		largeScale(largeScale),
		smallScale(smallScale),
		lineSpacingLarge(lineSpacingLarge),
		lineSpacingSmall(lineSpacingSmall)
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
	float GetLargeScale() const
	{
		return largeScale;
	}
	float GetSmallScale() const
	{
		return smallScale;
	}
	float GetLargeSpacing() const
	{
		return lineSpacingLarge;
	}
	float GetSmallSpacing() const
	{
		return lineSpacingSmall;
	}
private:
	size_t serverPort;
	float largeScale;
	float smallScale;
	float lineSpacingLarge;
	float lineSpacingSmall;
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
	void StringControl(const std::string& ctrlStr, Color& colChange);
private:
	Time time;
	FPS fps;
	
	bool returnToSetup = false;

	//network
	std::unique_ptr<CustomServer> server;
	int netLooper = 58;
	std::string oldInfo = "No Messages";

	//text control
	Color textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	Color oldTextCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	float largeScale = 1.0f;
	float smallScale = 0.35f;
	float currScale = smallScale;
	float lineSpacing = 1.0f;
	bool currSmall = true;
	float rotation = 0.0f;
	char buffer[512] = { '\0' };
	std::wstring oldMessage;
	bool holdingLastMsg = false;
	float alpha = 1.0f;
	float deltaZoom = 0.20f;
	float deltaAlpha = 1.0f;

	//simple Text
	Window& wnd;
	SimpleText st;
};


