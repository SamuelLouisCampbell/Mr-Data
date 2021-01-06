#pragma once

#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "Window.h"



class RMData
{
public: 
	RMData(size_t serverPort_in, const char* clientIP, float largeScale, 
		   float smallScale, float lineSpacingLarge, float lineSpacingSmall)
		:
		clientIP(clientIP),
		serverPort(serverPort_in),
		largeScale(largeScale),
		smallScale(smallScale),
		lineSpacingLarge(lineSpacingLarge),
		lineSpacingSmall(lineSpacingSmall)
	{}
	bool CheckRMIPGood()
	{
		const char* addrBuff; // Thrown away here :-(
		if (inet_pton(AF_INET, clientIP, &addrBuff) != 1)
			return false;
		else
			return true;
	}
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
	const char* GetIP() const
	{
		return clientIP;
	}
	void SetPortToDefault()
	{
		serverPort = 60000U;
	}
	void SetIPToDefault()
	{
		clientIP = "127.0.0.1";
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
	const char* clientIP;
	float largeScale;
	float smallScale;
	float lineSpacingLarge;
	float lineSpacingSmall;
};

class RenderMode
{
public:
	RenderMode(Graphics& gfx, RMData& data);
	~RenderMode();
	void Update(Window& wnd);
	void Render(Graphics& gfx);
	bool returnToSetupMode() const;
private:
	void StringControl(const std::string& ctrlStr, Color& colChange);
private:
	Time time;
	TextNode txt;
	FPS fps;
	DirectX::SimpleMath::Vector2 centre;
	bool returnToSetup = false;


	//text control
	Color textCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	Color oldTextCol = { 1.0f, 1.0f, 1.0f, 1.0f };
	float largeScale = 1.0f;
	float smallScale = 0.35f;
	float currScale = smallScale;
	float lineSpacingLarge = 1.65f;
	float lineSpacingSmall = 0.7f;
	float currLineSpacing = lineSpacingSmall;
	bool currSmall = true;
	float rotation = 0.0f;
	char buffer[512] = { '\0' };
	std::wstring oldMessage;
	bool holdingLastMsg = false;
	float alpha = 1.0f;
	float deltaZoom = 0.20f;
	float deltaAlpha = 1.0f;

};


