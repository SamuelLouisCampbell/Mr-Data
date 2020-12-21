#pragma once
#include "Time.h"
#include "BindableBase.h"
#include "TextNode.h"
#include "NDI_Send.h"
#include "Window.h"
#include "UDPClient.h"
#include "UDPServer.h"

class RMData
{
public: 
	RMData(size_t clientPort_in, size_t serverPort_in, const char* clientIP)
		:
		clientIP(clientIP),
		serverPort(serverPort_in),
		clientPort(clientPort_in)
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
		if (serverPort == clientPort)
			return false;
		if (serverPort >= std::numeric_limits<unsigned short>::max() || serverPort <= 0U)
			return false;
		if (clientPort >= std::numeric_limits<unsigned short>::max() || clientPort <= 0U)
			return false;
		else return true;
	}
	unsigned short GetClientPort() const
	{
		return unsigned short(clientPort);
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
		clientPort = 5000U;
		serverPort = 6000U;
	}
	void SetIPToDefault()
	{
		clientIP = "127.0.0.1";
	}
private:
	size_t clientPort;
	size_t serverPort;
	const char* clientIP;
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


	//UDP stuff
	std::thread serverThread;
	UDPClient echoClient;
	UDPServer udp_s;

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


